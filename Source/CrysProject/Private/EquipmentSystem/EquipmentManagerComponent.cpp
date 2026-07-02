// Copyright Soccertitan 2026


#include "EquipmentSystem/EquipmentManagerComponent.h"

#include "CrimAbilitySystemBlueprintFunctionLibrary.h"
#include "CrimAbilitySystemComponent.h"
#include "CrysBlueprintFunctionLibrary.h"
#include "CrysLogChannels.h"
#include "CrysNativeGameplayTags.h"
#include "InventoryBlueprintFunctionLibrary.h"
#include "InventoryGameplayTags.h"
#include "InventoryManagerComponent.h"
#include "AbilitySystem/Ability/Combat/CombatBlueprintFunctionLibrary.h"
#include "AbilitySystem/Ability/Combat/CombatSystemComponent.h"
#include "AbilitySystem/AttributeSet/PrimaryAttributeSet.h"
#include "EquipmentSystem/EquipmentDefinition.h"
#include "EquipmentSystem/EquipmentSystemBlueprintFunctionLibrary.h"
#include "EquipmentSystem/ItemDefinitionFragment_Equipment.h"
#include "JobSystem/JobContainer.h"
#include "JobSystem/JobDefinition.h"
#include "JobSystem/JobSystemBlueprintFunctionLibrary.h"
#include "JobSystem/JobSystemComponent.h"
#include "Net/UnrealNetwork.h"


UEquipmentManagerComponent::UEquipmentManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
	
	AllowedItemContainers.AddTag(FInventoryGameplayTags::Get().ItemContainer_Default);
	
	WeaponEquipSlots.AddTag(Crys::NativeGameplayTag::EquipSlot_Hand_Main);
	WeaponEquipSlots.AddTag(Crys::NativeGameplayTag::EquipSlot_Hand_Sub);
}

void UEquipmentManagerComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ThisClass, EquippedItemsContainer);
}

void UEquipmentManagerComponent::BeginPlay()
{
	Super::BeginPlay();
	CacheIsNetSimulated();
	
	SetJobSystemComponent(UJobSystemBlueprintFunctionLibrary::GetJobSystemComponent(GetOwner()));
	SetInventoryManagerComponent(UInventoryBlueprintFunctionLibrary::GetInventoryManagerComponent(GetOwner()));
	SetCombatSystemComponent(UCombatBlueprintFunctionLibrary::GetCombatSystemComponent(GetOwner()));
	
	TryBroadcastEquipmentManagerReady();
}

void UEquipmentManagerComponent::PreNetReceive()
{
	Super::PreNetReceive();
	CacheIsNetSimulated();
}

void UEquipmentManagerComponent::SetCrimAbilitySystem_Implementation(UCrimAbilitySystemComponent* InAbilitySystemComponent)
{
	if (AbilitySystemComponent == InAbilitySystemComponent)
	{
		return;
	}
	
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UPrimaryAttributeSet::GetLevelAttribute()).RemoveAll(this);
		AbilitySystemComponent->RegisterGameplayTagEvent(Crys::NativeGameplayTag::Ability_State_DualWield).RemoveAll(this);
	}
	
	AbilitySystemComponent = InAbilitySystemComponent;
	
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UPrimaryAttributeSet::GetLevelAttribute()).AddUObject(this, &UEquipmentManagerComponent::OnLevelAttributeChanged);
		bool bSuccess = false;
		BaseLevel = UCrimAbilitySystemBlueprintFunctionLibrary::EvaluateAttributeValueWithTagsUpToChannel(AbilitySystemComponent, 
			UPrimaryAttributeSet::GetLevelAttribute(), EGameplayModEvaluationChannel::Channel0, FGameplayTagContainer(),
			FGameplayTagContainer(), bSuccess);
		
		FOnGameplayEffectTagCountChanged::FDelegate Delegate;
		Delegate.BindUObject(this, &UEquipmentManagerComponent::OnDualWieldTagCountChanged);
		AbilitySystemComponent->RegisterAndCallGameplayTagEvent(Crys::NativeGameplayTag::Ability_State_DualWield, Delegate);
	}
}

void UEquipmentManagerComponent::EquipItem(FGameplayTag EquipSlot, const FItemInstanceHandle& Handle)
{
	if (!IsEquipmentManagerInitialized() || !EquipSlot.IsValid() || !Handle.IsValid())
	{
		return;
	}
	
	FItemInstance* ItemInstance = FindItem(Handle);
	if (!ItemInstance)
	{
		return;
	}
	
	if (!CanEquipItem(ItemInstance->GetItem()) || 
		!CanEquipItemInSlot(EquipSlot, ItemInstance->GetItem()) ||
		IsEquipSlotBlocked(EquipSlot))
	{
		return;
	}
	
	if (!HasAuthority())
	{
		Server_EquipItem(EquipSlot, Handle);
		return;
	}
	
	UnequipItemInternal(EquipSlot);
	
	// Unequips the item from an EquipmentManagerComponent if it's currently equipped.
	const FItemFragment_Equipment* ItemFragment_Equipment = ItemInstance->GetItemPtr()->GetPtr<FItem>()->FindFragmentByType<FItemFragment_Equipment>();
	if (UEquipmentManagerComponent* EquippedTo = ItemFragment_Equipment->GetEquipmentManagerComponent())
	{
		if (const FEquippedItem* EquippedItem = EquippedTo->EquippedItemsContainer.FindItemByHandle(FItemInstanceHandle(Handle)))
		{
			EquippedTo->UnequipItemInternal(EquippedItem->EquipSlot);
		}
	}
	
	EquipItemInternal(EquipSlot, ItemInstance);
}

void UEquipmentManagerComponent::UnequipItem(FGameplayTag EquipSlot)
{
	if (IsEquipmentManagerInitialized() && EquipSlot.IsValid())
	{
		if (const FEquippedItem* EquippedItem = EquippedItemsContainer.FindItemByEquipSlot(EquipSlot))
		{
			if (!HasAuthority())
			{
				Server_UnequipItem(EquipSlot);
				return;
			}
			
			UnequipItemInternal(EquipSlot);
		}
	}
}

FEquippedItem UEquipmentManagerComponent::GetEquippedItem(const FGameplayTag& EquipSlot) const
{
	if (FEquippedItem* EquippedItem = EquippedItemsContainer.FindItemByEquipSlot(EquipSlot))
	{
		return *EquippedItem;
	}
	return FEquippedItem();
}

bool UEquipmentManagerComponent::CanEquipItem(const TInstancedStruct<FItem>& Item) const
{
	const UEquipmentDefinition* EquipmentDef = UEquipmentSystemBlueprintFunctionLibrary::FindEquipmentDefinition(Item);
	if (!EquipmentDef)
	{
		return false;
	}
	
	if (EquipmentDef->LevelRequirement > 0 &&
		BaseLevel < EquipmentDef->LevelRequirement)
	{
		return false;
	}

	if (EquipmentDef->JobContainer && EquipmentDef->JobContainer->Jobs.IsValid())
	{
		if (MainJob)
		{
			if (!EquipmentDef->JobContainer->Jobs.HasTag(MainJob->JobTag))
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	
	return true;
}

bool UEquipmentManagerComponent::CanEquipItemInSlot(FGameplayTag EquipSlot, const TInstancedStruct<FItem>& Item) const
{
	if (!EquipSlot.IsValid())
	{
		return false;
	}
	
	const UEquipmentDefinition* EquipmentDef = UEquipmentSystemBlueprintFunctionLibrary::FindEquipmentDefinition(Item);
	if (!EquipmentDef)
	{
		return false;
	}
	
	if (!EquipSlot.MatchesTag(EquipmentDef->EquipSlot))
	{
		return false;
	}
		
	// Prevent Weapons from being equipped in the SubHand unless they have the DualWield tag.
	if (EquipSlot.MatchesTag(Crys::NativeGameplayTag::EquipSlot_Hand_Sub))
	{
		if (EquipmentDef->bWeapon)
		{
			if (bDualWield == false)
			{
				return false;
			}
		}
	}
	
	return true;
}

bool UEquipmentManagerComponent::IsEquipSlotBlocked(const FGameplayTag EquipSlot) const
{
	if (!EquipSlot.IsValid())
	{
		return true;
	}

	// Don't allow equipping the item if the slot is blocked by a currently equipped item.
	for (const FEquippedItem& EquippedItem : EquippedItemsContainer.Items)
	{
		if (EquippedItem.BlockedEquipSlots.HasTag(EquipSlot))
		{
			return true;
		}
	}

	return false;
}

bool UEquipmentManagerComponent::IsEquipmentManagerInitialized() const
{
	if (!AbilitySystemComponent)
	{
		return false;
	}
	
	if (!JobSystemComponent)
	{
		return false;
	}
	
	if (!InventoryManagerComponent)
	{
		return false;
	}
	
	if (!CombatSystemComponent)
	{
		return false;
	}
	
	return true;
}

bool UEquipmentManagerComponent::HasAuthority() const
{
	return !bCachedIsNetSimulated;
}

void UEquipmentManagerComponent::OnRegister()
{
	Super::OnRegister();
	CacheIsNetSimulated();
	EquippedItemsContainer.Owner = this;
}

void UEquipmentManagerComponent::OnItemEquipped(const FEquippedItem& EquippedItem)
{
	OnItemEquippedDelegate.Broadcast(EquippedItem);
}

void UEquipmentManagerComponent::OnItemUnequipped(const FEquippedItem& EquippedItem)
{
	OnItemUnequippedDelegate.Broadcast(EquippedItem);
}

void UEquipmentManagerComponent::OnItemRemovedFromContainer(const FItemInstance& ItemInstance)
{
	if (HasAuthority())
	{
		FItemInstanceHandle Handle(ItemInstance);
		if (FEquippedItem* EquippedItem = EquippedItemsContainer.FindItemByHandle(Handle))
		{
			// Unequip an item if was removed or moved to an invalid container.
			if (UItemContainer* MovedToItemContainer = ItemInstance.GetMovedToItemContainer())
			{
				if (MovedToItemContainer->GetInventoryManagerComponent() == InventoryManagerComponent)
				{
					if (AllowedItemContainers.IsEmpty() || AllowedItemContainers.HasTag(MovedToItemContainer->GetItemContainerTag()))
					{
						if (FItemInstance* NewItemInstance = MovedToItemContainer->FindItemByGuid(ItemInstance.GetGuid()))
						{
							FItemInstanceHandle NewHandle(*NewItemInstance);
							EquippedItem->ItemInstanceHandle = NewHandle;
							EquippedItemsContainer.MarkItemDirty(*EquippedItem);
						}
					}
					else
					{
						UnequipItemInternal(EquippedItem->EquipSlot);
					}
				}
				else
				{
					UnequipItemInternal(EquippedItem->EquipSlot);
				}
			}
			else
			{
				UnequipItemInternal(EquippedItem->EquipSlot);
			}
		}
	}
}

void UEquipmentManagerComponent::OnItemChangedInContainer(const FItemInstance& ItemInstance)
{
	if (HasAuthority())
	{
		FItemInstanceHandle Handle(ItemInstance);
		if (FEquippedItem* EquippedItem = EquippedItemsContainer.FindItemByHandle(Handle))
		{
			int32 ItemLevel = ItemInstance.GetItem().Get().GameplayTagStackContainer.GetStackCount(Crys::NativeGameplayTag::Item_UpgradeLevel);
			AbilitySystemComponent->SetActiveGameplayEffectLevel(EquippedItem->GameplayEffectHandle, ItemLevel);
			
			if (WeaponEquipSlots.HasTag(EquippedItem->EquipSlot))
			{
				SetWeapon(EquippedItem->EquipSlot, ItemInstance);
			}
		}
	}
}

void UEquipmentManagerComponent::OnMainJobChanged(UJobDefinition* JobDefinition)
{
	MainJob = JobDefinition;
	bMainJobChanged = true;
}

void UEquipmentManagerComponent::OnChangingJobs(bool bInChangingJobs)
{
	bChangingJobs = bInChangingJobs;
	
	if (bChangingJobs == false)
	{
		const bool bBaseLevelLower = PreJobChangeLevel > BaseLevel;
		if (bMainJobChanged || bBaseLevelLower)
		{
			UnequipInvalidItems();
		}
		PreJobChangeLevel = -1;
		bMainJobChanged = false;
	}
	else
	{
		// Changing Jobs cache the level at start of the change.
		PreJobChangeLevel = BaseLevel;
	}
}

void UEquipmentManagerComponent::CacheIsNetSimulated()
{
	bCachedIsNetSimulated = IsNetSimulating();
}

void UEquipmentManagerComponent::SetInventoryManagerComponent(UInventoryManagerComponent* InventoryManager)
{
	if (InventoryManagerComponent == InventoryManager)
	{
		return;
	}
	
	if (InventoryManagerComponent && HasAuthority())
	{
		InventoryManagerComponent->OnItemRemovedDelegate.RemoveAll(this);
		InventoryManagerComponent->OnItemChangedDelegate.RemoveAll(this);
	}
	
	InventoryManagerComponent = InventoryManager;
	
	if (InventoryManagerComponent && HasAuthority())
	{
		InventoryManagerComponent->OnItemRemovedDelegate.AddUniqueDynamic(this, &UEquipmentManagerComponent::OnItemRemovedFromContainer);
		InventoryManagerComponent->OnItemChangedDelegate.AddUniqueDynamic(this, &UEquipmentManagerComponent::OnItemChangedInContainer);
	}
}

void UEquipmentManagerComponent::SetJobSystemComponent(UJobSystemComponent* JobSystem)
{
	if (JobSystemComponent == JobSystem)
	{
		return;
	}
	
	if (JobSystemComponent)
	{
		MainJob = nullptr;
		JobSystemComponent->OnMainJobChangedDelegate.RemoveAll(this);
		if (HasAuthority())
		{
			JobSystemComponent->OnChangingJobsDelegate.RemoveAll(this);
		}
	}
	
	JobSystemComponent = JobSystem;
	
	if (JobSystemComponent)
	{
		MainJob = JobSystemComponent->GetMainJob();
		JobSystemComponent->OnMainJobChangedDelegate.AddUniqueDynamic(this, &UEquipmentManagerComponent::OnMainJobChanged);
		if (HasAuthority())
		{
			JobSystemComponent->OnChangingJobsDelegate.AddUniqueDynamic(this, &UEquipmentManagerComponent::OnChangingJobs);
		}
	}
}

void UEquipmentManagerComponent::SetCombatSystemComponent(UCombatSystemComponent* CombatSystem)
{
	if (CombatSystemComponent == CombatSystem)
	{
		return;
	}
	
	CombatSystemComponent = CombatSystem;
}

void UEquipmentManagerComponent::TryBroadcastEquipmentManagerReady()
{
	if (IsEquipmentManagerInitialized() && bBroadcastedIsReady == false)
	{
		OnEquipmentManagerInitializedDelegate.Broadcast();
		bBroadcastedIsReady = true;
	}
	else
	{
		checkf(false, TEXT("EquipmentManagerComponent is not initialized properly on [%s]"), *GetNameSafe(GetOwner()));
	}
}

void UEquipmentManagerComponent::OnLevelAttributeChanged(const FOnAttributeChangeData& Data)
{
	bool bSuccess = false;
	int32 NewBaseLevel = UCrimAbilitySystemBlueprintFunctionLibrary::EvaluateAttributeValueWithTagsUpToChannel(AbilitySystemComponent, 
		UPrimaryAttributeSet::GetLevelAttribute(), EGameplayModEvaluationChannel::Channel0, FGameplayTagContainer(),
		FGameplayTagContainer(), bSuccess);
	
	if (BaseLevel != NewBaseLevel)
	{
		bool bCheckIfItemsCanBeEquipped = false;
		if (NewBaseLevel < BaseLevel)
		{
			bCheckIfItemsCanBeEquipped = true;
		}
		
		BaseLevel = NewBaseLevel;
		
		if (bCheckIfItemsCanBeEquipped && bChangingJobs == false)
		{
			UnequipInvalidItems();
		}
	}
}

void UEquipmentManagerComponent::OnDualWieldTagCountChanged(FGameplayTag Tag, int32 NewCount)
{
	bool bCanDualWield = NewCount > 0;
	if (bDualWield && bCanDualWield == false)
	{
		if (const FEquippedItem* EquippedItem = EquippedItemsContainer.FindItemByEquipSlot(Crys::NativeGameplayTag::EquipSlot_Hand_Sub))
		{
			if (FItemInstance* ItemInstance = FindItem(EquippedItem->ItemInstanceHandle))
			{
				UEquipmentDefinition* EquipmentDef = UEquipmentSystemBlueprintFunctionLibrary::FindEquipmentDefinition(ItemInstance->GetItem());
				if (EquipmentDef->bWeapon)
				{
					ClearWeapon(Crys::NativeGameplayTag::EquipSlot_Hand_Sub);
				}
			}
		}
	}
	bDualWield = bCanDualWield;
}

FItemInstance* UEquipmentManagerComponent::FindItem(const FItemInstanceHandle& Handle) const
{
	if (Handle.IsValid())
	{
		UItemContainer* ItemContainer = Handle.GetItemContainer();
		if (ItemContainer->GetInventoryManagerComponent() == InventoryManagerComponent)
		{
			if (AllowedItemContainers.IsEmpty() || AllowedItemContainers.HasTag(ItemContainer->GetItemContainerTag()))
			{
				return ItemContainer->FindItemByGuid(Handle.GetGuid());
			}
		}
	}
	return nullptr;
}

void UEquipmentManagerComponent::UnequipInvalidItems()
{
	if (HasAuthority())
	{
		TArray<FGameplayTag> EquipSlotsToUnequip;
		EquipSlotsToUnequip.Reserve(EquippedItemsContainer.Items.Num());
	
		for (const FEquippedItem& EquippedItem : EquippedItemsContainer.Items)
		{
			FItemInstance* ItemInstance = FindItem(EquippedItem.ItemInstanceHandle);
			if (!CanEquipItem(ItemInstance->GetItem()))
			{
				EquipSlotsToUnequip.Add(EquippedItem.EquipSlot);
			}
		}
	
		for (const FGameplayTag& EquipSlot : EquipSlotsToUnequip)
		{
			UnequipItemInternal(EquipSlot);
		}
	}
}

FCrysWeapon UEquipmentManagerComponent::GetWeapon(const FItemInstance& ItemInstance, bool& bSuccess) const
{
	bSuccess = false;
	if (UEquipmentDefinition* EquipmentDef = UEquipmentSystemBlueprintFunctionLibrary::FindEquipmentDefinition(ItemInstance.GetItem()))
	{
		if (EquipmentDef->bWeapon)
		{
			FCrysWeapon Weapon = EquipmentDef->Weapon;
			Weapon.SetLevel(ItemInstance.GetItem().Get().GameplayTagStackContainer.GetStackCount(Crys::NativeGameplayTag::Item_UpgradeLevel));
			bSuccess = true;
			return Weapon;
		}
	}
	return FCrysWeapon();
}

void UEquipmentManagerComponent::SetWeapon(const FGameplayTag& EquipSlot, const FItemInstance& ItemInstance)
{
	if (HasAuthority())
	{
		bool bSuccess = false;
		FCrysWeapon Weapon = GetWeapon(ItemInstance, bSuccess);
		if (bSuccess)
		{
			if (EquipSlot.MatchesTag(Crys::NativeGameplayTag::EquipSlot_Hand_Sub))
			{
				CombatSystemComponent->SetSecondaryWeaponOverride(Weapon);
				AbilitySystemComponent->SetLooseGameplayTagCount(Crys::NativeGameplayTag::Ability_State_DualWielding, 1, EGameplayTagReplicationState::TagOnly);
			}
			else
			{
				CombatSystemComponent->SetPrimaryWeaponOverride(Weapon);
			}
		}
	}
}

void UEquipmentManagerComponent::ClearWeapon(const FGameplayTag& EquipSlot)
{
	if (HasAuthority())
	{
		if (EquipSlot.MatchesTag(Crys::NativeGameplayTag::EquipSlot_Hand_Sub))
		{
			CombatSystemComponent->ClearSecondaryWeaponOverride();
			AbilitySystemComponent->SetLooseGameplayTagCount(Crys::NativeGameplayTag::Ability_State_DualWielding, 0, EGameplayTagReplicationState::TagOnly);
		}
		else
		{
			CombatSystemComponent->ClearPrimaryWeaponOverride();
		}
	}
}

void UEquipmentManagerComponent::EquipItemInternal(const FGameplayTag& EquipSlot, FItemInstance* ItemInstance)
{
	FEquippedItem NewEquippedItem;
	NewEquippedItem.EquipSlot = EquipSlot;
	NewEquippedItem.ItemInstanceHandle = FItemInstanceHandle(*ItemInstance);
	NewEquippedItem.GameplayEffectHandle = ApplyEquipmentGameplayEffect(ItemInstance->GetItem());
	NewEquippedItem.BlockedEquipSlots = UEquipmentSystemBlueprintFunctionLibrary::FindEquipmentDefinition(ItemInstance->GetItem())->BlockEquipSlots;
	
	if (WeaponEquipSlots.HasTag(EquipSlot))
	{
		SetWeapon(EquipSlot, *ItemInstance);
	}
	
	ItemInstance->GetItemPtr()->GetMutablePtr<FItem>()->FindMutableFragmentByType<FItemFragment_Equipment>()->EquipmentManagerComponent = this;
	ItemInstance->MarkItemDirty();
	
	FEquippedItem& EquippedItem = EquippedItemsContainer.Items.AddDefaulted_GetRef();
	EquippedItem = NewEquippedItem;
	OnItemEquipped(EquippedItem);
	EquippedItemsContainer.MarkItemDirty(EquippedItem);
}

void UEquipmentManagerComponent::UnequipItemInternal(const FGameplayTag& EquipSlot)
{
	for (int32 idx = EquippedItemsContainer.Items.Num() - 1; idx >= 0; idx--)
	{
		if (EquippedItemsContainer.Items[idx].EquipSlot == EquipSlot)
		{
			FEquippedItem TempRemovedItem = EquippedItemsContainer.Items[idx];
			EquippedItemsContainer.Items.RemoveAt(idx);
			AbilitySystemComponent->RemoveActiveGameplayEffect(TempRemovedItem.GameplayEffectHandle);
			
			if (WeaponEquipSlots.HasTag(EquipSlot))
			{
				ClearWeapon(EquipSlot);
			}
			
			if (FItemInstance* ItemInstance = TempRemovedItem.ItemInstanceHandle.GetItemInstance())
			{
				ItemInstance->GetItemPtr()->GetMutablePtr<FItem>()->FindMutableFragmentByType<FItemFragment_Equipment>()->EquipmentManagerComponent = nullptr;
				ItemInstance->MarkItemDirty();
			}
			
			OnItemUnequipped(TempRemovedItem);
			EquippedItemsContainer.MarkArrayDirty();
			return;
		}
	}
}

FActiveGameplayEffectHandle UEquipmentManagerComponent::ApplyEquipmentGameplayEffect(const TInstancedStruct<FItem>& Item)
{
	const int32 UpgradeLevel = Item.Get().GameplayTagStackContainer.GetStackCount(Crys::NativeGameplayTag::Item_UpgradeLevel);
	const UEquipmentDefinition* EquipmentDef = UEquipmentSystemBlueprintFunctionLibrary::FindEquipmentDefinition(Item);

	FActiveGameplayEffectHandle Result;

	const TSubclassOf<UGameplayEffect> EquipmentGE = EquipmentDef->GameplayEffect;
	if (!EquipmentGE)
	{
		UE_LOG(LogCrys, Warning, TEXT("GameplayEffect is invalid in [%s]"), *GetNameSafe(EquipmentDef));
		return Result;
	}

	FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();
	FGameplayEffectSpecHandle Spec = AbilitySystemComponent->MakeOutgoingSpec(EquipmentGE, UpgradeLevel, ContextHandle);

	if (Spec.IsValid())
	{
		Result = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
	}

	return Result;
}

void UEquipmentManagerComponent::ClearEquipmentManagerFromItemInstance(FItemInstance* ItemInstance)
{
	FItemFragment_Equipment* ItemFragment = ItemInstance->GetItemPtr()->GetMutablePtr<FItem>()->FindMutableFragmentByType<FItemFragment_Equipment>();
	if (ItemFragment->GetEquipmentManagerComponent() == this)
	{
		ItemFragment->EquipmentManagerComponent = nullptr;
		ItemInstance->MarkItemDirty();
	}
}

void UEquipmentManagerComponent::Server_EquipItem_Implementation(const FGameplayTag& EquipSlot, const FItemInstanceHandle& Handle)
{
	EquipItem(EquipSlot, Handle);
}

void UEquipmentManagerComponent::Server_UnequipItem_Implementation(const FGameplayTag& EquipSlot)
{
	UnequipItem(EquipSlot);
}

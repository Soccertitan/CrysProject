// Copyright Soccertitan 2026


#include "EquipmentSystem/UI/EquipmentManagerViewModel.h"

#include "InventoryBlueprintFunctionLibrary.h"
#include "InventoryManagerComponent.h"
#include "AbilitySystem/CrysAbilityBlueprintFunctionLibrary.h"
#include "AbilitySystem/Ability/Combat/CombatSystemComponent.h"
#include "EquipmentSystem/EquipmentManagerComponent.h"
#include "EquipmentSystem/EquipmentSystemBlueprintFunctionLibrary.h"
#include "EquipmentSystem/UI/EquippedItemViewModel.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"
#include "InventorySystem/UI/Filter/ItemInstanceViewModelFilter_EquipableItems.h"
#include "UI/InventoryUISubsystem.h"
#include "UI/InventoryViewModelBlueprintFunctionLibrary.h"
#include "UI/ViewModel/ItemContainerViewModel.h"
#include "UI/ViewModel/ItemInstanceViewModel.h"


UEquipmentManagerViewModel::UEquipmentManagerViewModel()
{
	EquippableItemsFilter = CreateDefaultSubobject<UItemInstanceViewModelFilter_EquipableItems>("Filter");
}

void UEquipmentManagerViewModel::InitializeViewModel(APlayerController* PlayerController)
{
	Super::InitializeViewModel(PlayerController);
	
	APlayerState* PlayerState = PlayerController->GetPlayerState<APlayerState>();
	EquipmentManagerComponent = UEquipmentSystemBlueprintFunctionLibrary::GetEquipmentManagerComponent(PlayerState);
	
	if (EquipmentManagerComponent)
	{
		EquipmentManagerComponent->OnItemEquippedDelegate.AddUniqueDynamic(this, &UEquipmentManagerViewModel::OnItemEquipped);
		EquipmentManagerComponent->OnItemUnequippedDelegate.AddUniqueDynamic(this, &UEquipmentManagerViewModel::OnItemUnequipped);
	}
	
	InventoryManagerComponent = UInventoryBlueprintFunctionLibrary::GetInventoryManagerComponent(PlayerState);
	if (InventoryManagerComponent)
	{
		InventoryManagerComponent->OnItemAddedDelegate.AddUniqueDynamic(this, &UEquipmentManagerViewModel::OnItemChanged);
		InventoryManagerComponent->OnItemChangedDelegate.AddUniqueDynamic(this, &UEquipmentManagerViewModel::OnItemChanged);
	}
	
	if (EquipmentManagerComponent && InventoryManagerComponent)
	{
		FGameplayTagContainer AllowedItemContainers = EquipmentManagerComponent->GetAllowedItemContainers();
		UInventoryUISubsystem* UISubsystem = GetWorld()->GetSubsystem<UInventoryUISubsystem>();
		for (UItemContainer* ItemContainer : InventoryManagerComponent->GetItemContainers())
		{
			bool bAllowed = AllowedItemContainers.Num() == 0;
			for (const FGameplayTag& AllowedTag : AllowedItemContainers)
			{
				if (ItemContainer->GetItemContainerTag() == AllowedTag)
				{
					bAllowed = true;
				}
			}
			
			if (bAllowed)
			{
				if (UItemContainerViewModel* ItemContainerViewModel = UISubsystem->CreateItemContainerViewModel(ItemContainer))
				{
					AllowedItemContainerViewModels.Add(ItemContainerViewModel);
				}
			}
		}
	}
}

UEquippedItemViewModel* UEquipmentManagerViewModel::FindOrCreateEquippedItemViewModel(const FGameplayTag EquipSlot)
{
	if (EquipSlot.IsValid())
	{
		for (TObjectPtr<UEquippedItemViewModel>& ViewModel : EquippedItemViewModels)
		{
			if (ViewModel->EquipSlot == EquipSlot)
			{
				return ViewModel;
			}
		}
		if (EquipmentManagerComponent)
		{
			return InternalCreateEquippedItemViewModel(EquipSlot);
		}
	}
	return nullptr;
}

TArray<UItemContainerViewModel*> UEquipmentManagerViewModel::GetAllowedItemContainers() const
{
	return AllowedItemContainerViewModels;
}

TArray<UItemInstanceViewModel*> UEquipmentManagerViewModel::GetEquippableItems(const FGameplayTag EquipSlot, UItemContainerViewModel* ItemContainerViewModel) const
{
	TArray<UItemInstanceViewModel*> Result;
	
	if (EquipSlot.IsValid() && ItemContainerViewModel)
	{
		Result = ItemContainerViewModel->GetItemInstanceViewModels();
		EquippableItemsFilter->EquipSlot = EquipSlot;
		EquippableItemsFilter->FilterItemInstanceViewModels(EquipmentManagerComponent, Result);
	}
	
	return Result;
}

void UEquipmentManagerViewModel::EquipItem(FGameplayTag EquipSlot, UItemInstanceViewModel* ItemInstanceViewModel)
{
	if (EquipmentManagerComponent && ItemInstanceViewModel)
	{
		EquipmentManagerComponent->EquipItem(EquipSlot, ItemInstanceViewModel->GetHandle());
	}
}

void UEquipmentManagerViewModel::UnequipItem(FGameplayTag EquipSlot)
{
	if (EquipmentManagerComponent)
	{
		EquipmentManagerComponent->UnequipItem(EquipSlot);
	}
}

UEquippedItemViewModel* UEquipmentManagerViewModel::InternalCreateEquippedItemViewModel(const FGameplayTag& EquipSlot)
{
	UEquippedItemViewModel* NewVM = NewObject<UEquippedItemViewModel>(this);
	NewVM->SetEquipSlot(EquipSlot);
	if (FItemInstance* ItemInstance = EquipmentManagerComponent->GetEquippedItem(EquipSlot).ItemInstanceHandle.GetItemInstance())
	{
		NewVM->SetItemInstanceViewModel(
		UInventoryViewModelBlueprintFunctionLibrary::CreateItemInstanceViewModel(this, *ItemInstance));	
	}
	EquippedItemViewModels.Add(NewVM);
	return NewVM;
}

void UEquipmentManagerViewModel::OnItemEquipped(const FEquippedItem& EquippedItem)
{
	for (UEquippedItemViewModel* ViewModel : EquippedItemViewModels)
	{
		if (ViewModel->GetEquipSlot() == EquippedItem.EquipSlot)
		{
			UItemInstanceViewModel* ItemInstanceViewModel = UInventoryViewModelBlueprintFunctionLibrary::CreateItemInstanceViewModel(
				this, *EquippedItem.ItemInstanceHandle.GetItemInstance());
			ViewModel->SetItemInstanceViewModel(ItemInstanceViewModel);
			break;
		}
	}
}

void UEquipmentManagerViewModel::OnItemUnequipped(const FEquippedItem& EquippedItem)
{
	for (UEquippedItemViewModel* ViewModel : EquippedItemViewModels)
	{
		if (ViewModel->GetEquipSlot() == EquippedItem.EquipSlot)
		{
			UItemInstanceViewModel* ItemInstanceViewModel = NewObject<UItemInstanceViewModel>(this);
			ViewModel->SetItemInstanceViewModel(ItemInstanceViewModel);
			break;
		}
	}
}

void UEquipmentManagerViewModel::OnItemChanged(const FItemInstance& ItemInstance)
{
	for (UEquippedItemViewModel* ViewModel : EquippedItemViewModels)
	{
		if (ViewModel->GetItemInstanceViewModel()->GetHandle().GetGuid() == ItemInstance.GetGuid())
		{
			ViewModel->GetItemInstanceViewModel()->SetItemInstance(ItemInstance);
		}
	}
}

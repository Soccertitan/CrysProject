// Copyright Soccertitan 2026


#include "EquipmentSystem/UI/EquipmentManagerViewModel.h"

#include "InventoryBlueprintFunctionLibrary.h"
#include "InventoryManagerComponent.h"
#include "InventorySettings.h"
#include "AbilitySystem/CrysAbilityBlueprintFunctionLibrary.h"
#include "Engine/World.h"
#include "EquipmentSystem/EquipmentManagerComponent.h"
#include "EquipmentSystem/EquipmentSystemBlueprintFunctionLibrary.h"
#include "EquipmentSystem/UI/EquippedItemViewModel.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"
#include "InventorySystem/UI/Filter/Filter_EquipableItems.h"
#include "UI/InventoryUISubsystem.h"
#include "UI/ViewModel/ItemContainerViewModel.h"
#include "UI/ViewModel/ItemInstanceViewModel.h"


UEquipmentManagerViewModel::UEquipmentManagerViewModel()
{
	EquippableItemsFilter = CreateDefaultSubobject<UFilter_EquipableItems>("Filter");
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
		
		for (int32 Index = Result.Num() - 1; Index >= 0; Index--)
		{
			if (EquippableItemsFilter->ShouldFilterObject(Result[Index], EquipmentManagerComponent))
			{
				Result.RemoveAt(Index);
			}
		}
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

UItemInstanceViewModel* UEquipmentManagerViewModel::FindItemInstanceViewModel(const FItemInstance* ItemInstance)
{
	if (ItemInstance)
	{
		for (const TObjectPtr<UItemContainerViewModel>& ContainerViewModel : AllowedItemContainerViewModels)
		{
			for (UItemInstanceViewModel* ItemInstanceViewModel : ContainerViewModel->GetItemInstanceViewModels())
			{
				if (ItemInstanceViewModel->GetHandle() == ItemInstance->GetHandle())
				{
					return ItemInstanceViewModel;
				}
			}
		}
	}
	UItemInstanceViewModel* NewViewModel = NewObject<UItemInstanceViewModel>(this, UInventorySettings::GetItemInstanceViewModelClass());
	if (ItemInstance)
	{
		NewViewModel->SetItemInstance(*ItemInstance);
	}
	return NewViewModel;
}

UEquippedItemViewModel* UEquipmentManagerViewModel::InternalCreateEquippedItemViewModel(const FGameplayTag& EquipSlot)
{
	UEquippedItemViewModel* NewVM = NewObject<UEquippedItemViewModel>(this);
	NewVM->SetEquipSlot(EquipSlot);
	UItemInstanceViewModel* ItemInstanceViewModel = FindItemInstanceViewModel(EquipmentManagerComponent->GetEquippedItem(EquipSlot).ItemInstanceHandle.GetItemInstance());
	NewVM->SetItemInstanceViewModel(ItemInstanceViewModel);
	EquippedItemViewModels.Add(NewVM);
	return NewVM;
}

void UEquipmentManagerViewModel::OnItemEquipped(const FEquippedItem& EquippedItem)
{
	for (UEquippedItemViewModel* ViewModel : EquippedItemViewModels)
	{
		if (ViewModel->GetEquipSlot() == EquippedItem.EquipSlot)
		{
			UItemInstanceViewModel* ItemInstanceViewModel = FindItemInstanceViewModel(EquippedItem.ItemInstanceHandle.GetItemInstance());
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
			UItemInstanceViewModel* ItemInstanceViewModel = NewObject<UItemInstanceViewModel>(this, UInventorySettings::GetItemInstanceViewModelClass());
			ViewModel->SetItemInstanceViewModel(ItemInstanceViewModel);
			break;
		}
	}
}

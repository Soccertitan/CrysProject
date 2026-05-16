// Copyright Soccertitan 2026


#include "InventorySystem/UI/Filter/ItemInstanceViewModelFilter_EquipableItems.h"

#include "EquipmentSystem/EquipmentManagerComponent.h"
#include "UI/ViewModel/ItemInstanceViewModel.h"


bool UItemInstanceViewModelFilter_EquipableItems::ShouldBeginFilter(const UObject* Context, TArray<UItemInstanceViewModel*>& ViewModels) const
{
	if (!Super::ShouldBeginFilter(Context, ViewModels))
	{
		return false;
	}

	if (!EquipSlot.IsValid())
	{
		return false;
	}

	if (!Cast<UEquipmentManagerComponent>(Context))
	{
		return false;
	}

	return true;
}

bool UItemInstanceViewModelFilter_EquipableItems::DoesItemInstanceViewModelPassFilter(const UObject* Context, UItemInstanceViewModel* ViewModel) const
{
	if (!Super::DoesItemInstanceViewModelPassFilter(Context, ViewModel))
	{
		return false;
	}

	const UEquipmentManagerComponent* EquipmentManager = Cast<UEquipmentManagerComponent>(Context);
	bool bCanEquipItem = EquipmentManager->CanEquipItem(ViewModel->GetItem());
	bool bCanEquipItemInSlot = EquipmentManager->CanEquipItemInSlot(EquipSlot, ViewModel->GetItem());
	return bCanEquipItem && bCanEquipItemInSlot;
}

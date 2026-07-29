// Copyright Soccertitan 2026


#include "InventorySystem/UI/Filter/Filter_EquipableItems.h"

#include "EquipmentSystem/EquipmentManagerComponent.h"
#include "UI/ViewModel/ItemInstanceViewModel.h"


bool UFilter_EquipableItems::ShouldFilterObject_Implementation(const UObject* Object, const UObject* Context) const
{
	if (!EquipSlot.IsValid())
	{
		return true;
	}
	
	const UEquipmentManagerComponent* EquipmentManager = Cast<UEquipmentManagerComponent>(Object);
	if (!EquipmentManager)
	{
		return true;
	}
	
	const UItemInstanceViewModel* ViewModel = Cast<UItemInstanceViewModel>(Object);
	
	if (!ViewModel)
	{
		return true;
	}
	
	bool bCanEquipItem = EquipmentManager->CanEquipItem(ViewModel->GetItem());
	bool bCanEquipItemInSlot = EquipmentManager->CanEquipItemInSlot(EquipSlot, ViewModel->GetItem());
	return !(bCanEquipItem && bCanEquipItemInSlot);
}

// Copyright Soccertitan 2026


#include "EquipmentSystem/UI/EquippedItemViewModel.h"

#include "UI/ViewModel/UITagViewModel.h"

void UEquippedItemViewModel::SetEquipSlot(const FGameplayTag& InValue)
{
	if (EquipSlot != InValue)
	{
		EquipSlot = InValue;
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(EquipSlot);
		
		UUITagViewModel* NewVM = NewObject<UUITagViewModel>(this);
		NewVM->SetGameplayTag(EquipSlot);
		SetEquipSlotGameplayTagViewModel(NewVM);
	}
}

void UEquippedItemViewModel::SetItemInstanceViewModel(UItemInstanceViewModel* InValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(ItemInstanceViewModel, InValue);
}

void UEquippedItemViewModel::SetEquipSlotGameplayTagViewModel(UUITagViewModel* InValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(EquipSlotUITagViewModel, InValue);
}

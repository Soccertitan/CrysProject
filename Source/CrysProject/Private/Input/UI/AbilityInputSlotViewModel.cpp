// Copyright Soccertitan 2026


#include "Input/UI/AbilityInputSlotViewModel.h"

void UAbilityInputSlotViewModel::SetInputSlot(const FAbilityInputSlot& InValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(InputSlot, InValue);
}

void UAbilityInputSlotViewModel::SetInputSet(const int32 InValue)
{
	InputSet = InValue;
}

void UAbilityInputSlotViewModel::SetAbilityViewModel(UAbilityViewModel* InValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(AbilityViewModel, InValue);
}

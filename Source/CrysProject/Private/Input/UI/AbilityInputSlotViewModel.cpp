// Copyright Soccertitan 2026


#include "Input/UI/AbilityInputSlotViewModel.h"

#include "Input/AbilityInputManagerComponent.h"
#include "Input/UI/AbilityInputManagerViewModel.h"
#include "Input/UI/InputActionManagerViewModel.h"

void UAbilityInputSlotViewModel::InputSlotPressed() const
{
	if (UAbilityInputManagerViewModel* AIMV = Cast<UAbilityInputManagerViewModel>(GetOuter()))
	{
		if (AIMV->AbilityInputManagerComponent)
		{
			AIMV->AbilityInputManagerComponent->InputSlotPressed(InputSlot, InputSet);
		}
	}
}

void UAbilityInputSlotViewModel::InputSlotReleased() const
{
	if (UAbilityInputManagerViewModel* AIMV = Cast<UAbilityInputManagerViewModel>(GetOuter()))
	{
		if (AIMV->AbilityInputManagerComponent)
		{
			AIMV->AbilityInputManagerComponent->InputSlotReleased(InputSlot, InputSet);
		}
	}
}

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

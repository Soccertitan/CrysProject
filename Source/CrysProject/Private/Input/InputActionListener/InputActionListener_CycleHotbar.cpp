// Copyright Soccertitan 2026


#include "Input/InputActionListener/InputActionListener_CycleHotbar.h"

#include "InputActionValue.h"
#include "Input/AbilityInputManagerComponent.h"

void UInputActionListener_CycleHotbar::InputActionTriggered(const FInputActionValue& Value)
{
	Super::InputActionTriggered(Value);
	
	if (AbilityInputManagerComponent)
	{
		const bool bCycleForward = Value.GetMagnitude() > 0.f;
		AbilityInputManagerComponent->SwitchToNextAbilityInputSet(bCycleForward);
	}
}

void UInputActionListener_CycleHotbar::Initialize()
{
	Super::Initialize();
	
	AbilityInputManagerComponent = GetPlayerController()->FindComponentByClass<UAbilityInputManagerComponent>();
}

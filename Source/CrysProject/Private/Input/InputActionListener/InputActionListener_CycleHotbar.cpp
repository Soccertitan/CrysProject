// Copyright Soccertitan 2026


#include "Input/InputActionListener/InputActionListener_CycleHotbar.h"

#include "InputActionValue.h"
#include "ActionSystem/CrysActionManagerComponent.h"

void UInputActionListener_CycleHotbar::InputActionTriggered(const FInputActionValue& Value)
{
	Super::InputActionTriggered(Value);
	
	if (ActionManagerComponent)
	{
		const bool bCycleForward = Value.GetMagnitude() > 0.f;
		ActionManagerComponent->SwitchToNextActionSet(bCycleForward);
	}
}

void UInputActionListener_CycleHotbar::Initialize()
{
	Super::Initialize();
	
	ActionManagerComponent = GetPlayerController()->FindComponentByClass<UCrysActionManagerComponent>();
}

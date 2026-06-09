// Copyright Soccertitan 2026


#include "Input/InputActionListener/InputActionListener_CrysAction.h"

#include "ActionSystem/CrysActionManagerComponent.h"


void UInputActionListener_CrysAction::Initialize()
{
	Super::Initialize();
	
	ActionManager = GetPlayerController()->FindComponentByClass<UCrysActionManagerComponent>();
}

void UInputActionListener_CrysAction::InputActionTriggered(const FInputActionValue& Value)
{
	Super::InputActionTriggered(Value);
	
	if (IsPressed() && ActionManager)
	{
		ActionManager->TryActivateActiveAction(InputTag, ActionIndex);
	}
}
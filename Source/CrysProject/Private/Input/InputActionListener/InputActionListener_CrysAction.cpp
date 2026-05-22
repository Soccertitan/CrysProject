// Copyright Soccertitan 2026


#include "Input/InputActionListener/InputActionListener_CrysAction.h"

#include "ActionSystem/CrysActionManagerComponent.h"


void UInputActionListener_CrysAction::OnInitializeListener()
{
	Super::OnInitializeListener();
	
	ActionManager = GetPlayerController()->FindComponentByClass<UCrysActionManagerComponent>();
}

void UInputActionListener_CrysAction::OnInputActionTriggered(const FInputActionValue& Value)
{
	Super::OnInputActionTriggered(Value);
	
	if (IsPressed() && ActionManager)
	{
		ActionManager->TryActivateAction(InputTag);
	}
}
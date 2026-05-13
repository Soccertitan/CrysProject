// Copyright Soccertitan 2026


#include "Input/InputActionListener/ActionInputActionListener.h"

#include "ActionSystem/CrysActionManagerComponent.h"


void UActionInputActionListener::OnInitializeListener()
{
	Super::OnInitializeListener();
	
	ActionManager = GetPlayerController()->FindComponentByClass<UCrysActionManagerComponent>();
}

void UActionInputActionListener::OnInputActionTriggered(const FInputActionValue& Value)
{
	Super::OnInputActionTriggered(Value);
	
	if (IsPressed() && ActionManager)
	{
		ActionManager->TryActivateAction(InputTag);
	}
}
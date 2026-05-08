// Copyright Soccertitan 2025


#include "Input/InputMappingContextInputActionListener.h"

#include "EnhancedInputSubsystems.h"

void UInputMappingContextInputActionListener::OnInitializeListener()
{
	Super::OnInitializeListener();
	
	EnhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetPlayerController()->GetLocalPlayer());
	ensure(EnhancedInputSubsystem);
}

void UInputMappingContextInputActionListener::OnInputActionTriggered(const FInputActionValue& Value)
{
	Super::OnInputActionTriggered(Value);
	
	if (InputMappingContext && bAppliedContext == false)
	{
		EnhancedInputSubsystem->AddMappingContext(InputMappingContext, Priority, ContextOptionsOnAdd);
		bAppliedContext = true;
	}
}

void UInputMappingContextInputActionListener::OnInputActionCompleted(const FInputActionValue& Value)
{
	Super::OnInputActionCompleted(Value);

	EnhancedInputSubsystem->RemoveMappingContext(InputMappingContext, ContextOptionsOnRemove);
	bAppliedContext = false;
}

void UInputMappingContextInputActionListener::OnInputActionCanceled(const FInputActionValue& Value)
{
	Super::OnInputActionCanceled(Value);
	
	EnhancedInputSubsystem->RemoveMappingContext(InputMappingContext, ContextOptionsOnRemove);
	bAppliedContext = false;
}

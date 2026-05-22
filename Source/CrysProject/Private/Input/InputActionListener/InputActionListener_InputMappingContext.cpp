// Copyright Soccertitan 2025


#include "Input/InputActionListener/InputActionListener_InputMappingContext.h"

#include "EnhancedInputSubsystems.h"

void UInputActionListener_InputMappingContext::OnInitializeListener()
{
	Super::OnInitializeListener();
	
	EnhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetPlayerController()->GetLocalPlayer());
	ensure(EnhancedInputSubsystem);
}

void UInputActionListener_InputMappingContext::OnInputActionTriggered(const FInputActionValue& Value)
{
	Super::OnInputActionTriggered(Value);
	
	if (InputMappingContext && bAppliedContext == false)
	{
		EnhancedInputSubsystem->AddMappingContext(InputMappingContext, Priority, ContextOptionsOnAdd);
		bAppliedContext = true;
	}
}

void UInputActionListener_InputMappingContext::OnInputActionCompleted(const FInputActionValue& Value)
{
	Super::OnInputActionCompleted(Value);

	EnhancedInputSubsystem->RemoveMappingContext(InputMappingContext, ContextOptionsOnRemove);
	bAppliedContext = false;
}

void UInputActionListener_InputMappingContext::OnInputActionCanceled(const FInputActionValue& Value)
{
	Super::OnInputActionCanceled(Value);
	
	EnhancedInputSubsystem->RemoveMappingContext(InputMappingContext, ContextOptionsOnRemove);
	bAppliedContext = false;
}

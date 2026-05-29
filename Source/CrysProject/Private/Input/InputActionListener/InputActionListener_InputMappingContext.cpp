// Copyright Soccertitan 2025


#include "Input/InputActionListener/InputActionListener_InputMappingContext.h"

#include "EnhancedInputSubsystems.h"

void UInputActionListener_InputMappingContext::Initialize()
{
	Super::Initialize();
	
	EnhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetPlayerController()->GetLocalPlayer());
	ensure(EnhancedInputSubsystem);
}

void UInputActionListener_InputMappingContext::InputActionTriggered(const FInputActionValue& Value)
{
	Super::InputActionTriggered(Value);
	
	if (InputMappingContext && bAppliedContext == false)
	{
		EnhancedInputSubsystem->AddMappingContext(InputMappingContext, Priority, ContextOptionsOnAdd);
		bAppliedContext = true;
	}
}

void UInputActionListener_InputMappingContext::InputActionCompleted(const FInputActionValue& Value)
{
	Super::InputActionCompleted(Value);

	EnhancedInputSubsystem->RemoveMappingContext(InputMappingContext, ContextOptionsOnRemove);
	bAppliedContext = false;
}

void UInputActionListener_InputMappingContext::InputActionCanceled(const FInputActionValue& Value)
{
	Super::InputActionCanceled(Value);
	
	EnhancedInputSubsystem->RemoveMappingContext(InputMappingContext, ContextOptionsOnRemove);
	bAppliedContext = false;
}

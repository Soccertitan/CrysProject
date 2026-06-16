// Copyright Soccertitan 2025


#include "Input/InputActionListener/InputActionListener_GameplayAbility.h"

#include "CrimAbilitySystemBlueprintFunctionLibrary.h"
#include "InputActionValue.h"
#include "Input/AbilityInputManagerComponent.h"


#if WITH_EDITOR
void UInputActionListener_GameplayAbility::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	
	if (bUseInputSlot)
	{
		AbilityInput = nullptr;
	}
	else
	{
		InputSlot = FAbilityInputSlot();
	}
}
#endif

void UInputActionListener_GameplayAbility::Initialize()
{
	Super::Initialize();
	
	AbilityInputManagerComponent = UCrimAbilitySystemBlueprintFunctionLibrary::GetAbilityInputManagerComponent(GetPlayerController());
}

void UInputActionListener_GameplayAbility::InputActionTriggered(const FInputActionValue& Value)
{
	Super::InputActionTriggered(Value);
	
	InternalAbilityInputPressed();
}

void UInputActionListener_GameplayAbility::InputActionCanceled(const FInputActionValue& Value)
{
	Super::InputActionCanceled(Value);
	
	InternalAbilityInputReleased();
}

void UInputActionListener_GameplayAbility::InputActionCompleted(const FInputActionValue& Value)
{
	Super::InputActionCompleted(Value);
	
	InternalAbilityInputReleased();
}

void UInputActionListener_GameplayAbility::InternalAbilityInputPressed()
{
	if (AbilityInputManagerComponent)
	{
		if (bUseInputSlot)
		{
			AbilityInputManagerComponent->InputSlotPressed(InputSlot, InputSet);
		}
		else
		{
			AbilityInputManagerComponent->InputPressed(AbilityInput);
		}
	}
}

void UInputActionListener_GameplayAbility::InternalAbilityInputReleased()
{
	if (AbilityInputManagerComponent)
	{
		if (bUseInputSlot)
		{
			AbilityInputManagerComponent->InputSlotReleased(InputSlot, InputSet);
		}
		else
		{
			AbilityInputManagerComponent->InputReleased(AbilityInput);
		}
	}
}

// Copyright Soccertitan 2025


#include "Input/InputActionListener/InputActionListener_GameplayAbility.h"

#include "CrimAbilitySystemBlueprintFunctionLibrary.h"
#include "InputActionValue.h"
#include "Abilities/GameplayAbility.h"
#include "Input/AbilityInputManagerComponent.h"

void UInputActionListener_GameplayAbility::OnInitializeListener()
{
	Super::OnInitializeListener();
	
	AbilityInputManagerComponent = UCrimAbilitySystemBlueprintFunctionLibrary::GetAbilityInputManagerComponent(GetPlayerController());
}

void UInputActionListener_GameplayAbility::OnInputActionTriggered(const FInputActionValue& Value)
{
	Super::OnInputActionTriggered(Value);
	
	if (AbilityInputManagerComponent)
	{
		if (Value.Get<bool>())
		{
			AbilityInputManagerComponent->InputPressed(AbilityClass.Get());
		}
		else
		{
			AbilityInputManagerComponent->InputReleased(AbilityClass.Get());
		}
	}
}

void UInputActionListener_GameplayAbility::OnInputActionCanceled(const FInputActionValue& Value)
{
	Super::OnInputActionCanceled(Value);
	
	if (AbilityInputManagerComponent)
	{
		AbilityInputManagerComponent->InputReleased(AbilityClass.Get());
	}
}

void UInputActionListener_GameplayAbility::OnInputActionCompleted(const FInputActionValue& Value)
{
	Super::OnInputActionCompleted(Value);
	
	if (AbilityInputManagerComponent)
	{
		AbilityInputManagerComponent->InputReleased(AbilityClass.Get());
	}
}

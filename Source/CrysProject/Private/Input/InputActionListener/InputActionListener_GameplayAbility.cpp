// Copyright Soccertitan 2025


#include "Input/InputActionListener/InputActionListener_GameplayAbility.h"

#include "CrimAbilitySystemBlueprintFunctionLibrary.h"
#include "InputActionValue.h"
#include "Abilities/GameplayAbility.h"
#include "Input/AbilityInputManagerComponent.h"

void UInputActionListener_GameplayAbility::Initialize()
{
	Super::Initialize();
	
	AbilityInputManagerComponent = UCrimAbilitySystemBlueprintFunctionLibrary::GetAbilityInputManagerComponent(GetPlayerController());
}

void UInputActionListener_GameplayAbility::InputActionTriggered(const FInputActionValue& Value)
{
	Super::InputActionTriggered(Value);
	
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

void UInputActionListener_GameplayAbility::InputActionCanceled(const FInputActionValue& Value)
{
	Super::InputActionCanceled(Value);
	
	if (AbilityInputManagerComponent)
	{
		AbilityInputManagerComponent->InputReleased(AbilityClass.Get());
	}
}

void UInputActionListener_GameplayAbility::InputActionCompleted(const FInputActionValue& Value)
{
	Super::InputActionCompleted(Value);
	
	if (AbilityInputManagerComponent)
	{
		AbilityInputManagerComponent->InputReleased(AbilityClass.Get());
	}
}

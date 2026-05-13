// Copyright Soccertitan 2025


#include "Input/InputActionListener/GameplayAbilityInputActionListener.h"

#include "CrimAbilitySystemBlueprintFunctionLibrary.h"
#include "InputActionValue.h"
#include "Abilities/GameplayAbility.h"
#include "Input/AbilityInputManagerComponent.h"

void UGameplayAbilityInputActionListener::OnInitializeListener()
{
	Super::OnInitializeListener();
	
	AbilityInputManagerComponent = UCrimAbilitySystemBlueprintFunctionLibrary::GetAbilityInputManagerComponent(GetPlayerController());
}

void UGameplayAbilityInputActionListener::OnInputActionTriggered(const FInputActionValue& Value)
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

void UGameplayAbilityInputActionListener::OnInputActionCanceled(const FInputActionValue& Value)
{
	Super::OnInputActionCanceled(Value);
	
	if (AbilityInputManagerComponent)
	{
		AbilityInputManagerComponent->InputReleased(AbilityClass.Get());
	}
}

void UGameplayAbilityInputActionListener::OnInputActionCompleted(const FInputActionValue& Value)
{
	Super::OnInputActionCompleted(Value);
	
	if (AbilityInputManagerComponent)
	{
		AbilityInputManagerComponent->InputReleased(AbilityClass.Get());
	}
}

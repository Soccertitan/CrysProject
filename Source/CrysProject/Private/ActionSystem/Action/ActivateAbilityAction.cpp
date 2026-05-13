// Copyright Soccertitan 2026


#include "ActionSystem/Action/ActivateAbilityAction.h"

#include "Ability/CrimGameplayAbility.h"
#include "Input/AbilityInputManagerComponent.h"
#include "Player/CrysPlayerController.h"


bool UActivateAbilityAction::CanActivateAction() const
{
	if (Super::CanActivateAction())
	{
		if (!AbilityInputComponent)
		{
			return false;
		}
	}
	
	return true;
}

void UActivateAbilityAction::InitializeAction()
{
	Super::InitializeAction();
	
	if (GetPlayerController())
	{
		AbilityInputComponent = GetPlayerController()->FindComponentByClass<UAbilityInputManagerComponent>();
	}
}

void UActivateAbilityAction::ActivateAction_Implementation()
{
	Super::ActivateAction_Implementation();
	
	AbilityInputComponent->InputPressed(AbilityClass.Get());
	AbilityInputComponent->InputReleased(AbilityClass.Get());
}
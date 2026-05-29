// Copyright Soccertitan 2025


#include "Input/InputActionListener/InputActionListener_StopAttackClearTarget.h"

#include "CrimTargetingSystemBlueprintFunctionLibrary.h"
#include "CrimTargetingSystemComponent.h"
#include "InputActionValue.h"
#include "AbilitySystem/CrysAbilityBlueprintFunctionLibrary.h"
#include "AbilitySystem/Ability/Combat/CombatSystemComponent.h"
#include "GameFramework/PlayerState.h"


void UInputActionListener_StopAttackClearTarget::Initialize()
{
	Super::Initialize();
	
	GetCombatSystem();
}

void UInputActionListener_StopAttackClearTarget::OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn)
{
	Super::OnPossessedPawnChanged(OldPawn, NewPawn);
	
	TargetingSystemComponent = UCrimTargetingSystemBlueprintFunctionLibrary::GetCrimTargetingSystemComponent(NewPawn);
}

void UInputActionListener_StopAttackClearTarget::InputActionTriggered(const FInputActionValue& Value)
{
	Super::InputActionTriggered(Value);
	
	if (Value.Get<bool>() == false)
	{
		return;
	}
	
	if (GetCombatSystem() && CombatSystemComponent->IsAutoAttacking())
	{
		CombatSystemComponent->StopAutoAttack();
		return;
	}
	
	if (TargetingSystemComponent)
	{
		if (TargetingSystemComponent->IsLockedOn())
		{
			TargetingSystemComponent->SetLockOnState(false);
			return;
		}
		if (TargetingSystemComponent->GetTargetPoint().GetActor())
		{
			TargetingSystemComponent->ClearTargetPoint();
			return;
		}
	}
}

UCombatSystemComponent* UInputActionListener_StopAttackClearTarget::GetCombatSystem()
{
	if (!CombatSystemComponent)
	{
		CombatSystemComponent = UCrysAbilityBlueprintFunctionLibrary::GetCombatSystemComponent(GetPlayerController()->GetPlayerState<APlayerState>());
	}
	
	return CombatSystemComponent;
}

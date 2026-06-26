// Copyright Soccertitan 2025


#include "Input/InputActionListener/InputActionListener_AttackInteract.h"

#include "CrimAbilitySystemBlueprintFunctionLibrary.h"
#include "CrimAbilitySystemComponent.h"
#include "CrimTargetingSystemBlueprintFunctionLibrary.h"
#include "CrimTargetingSystemComponent.h"
#include "CrysBlueprintFunctionLibrary.h"
#include "CrysNativeGameplayTags.h"
#include "GenericTeamAgentInterface.h"
#include "InputActionValue.h"
#include "InteractionSystemBlueprintFunctionLibrary.h"
#include "InteractorComponent.h"
#include "AbilitySystem/Ability/Combat/CombatBlueprintFunctionLibrary.h"
#include "AbilitySystem/Ability/Combat/CombatSystemComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"

UInputActionListener_AttackInteract::UInputActionListener_AttackInteract()
{
}

void UInputActionListener_AttackInteract::Initialize()
{
	Super::Initialize();
	
	GetCombatSystemComponent();
	GetAbilitySystemComponent();
	TargetingSystemComponent = UCrimTargetingSystemBlueprintFunctionLibrary::GetCrimTargetingSystemComponent(GetPlayerController());
}

void UInputActionListener_AttackInteract::InputActionTriggered(const FInputActionValue& Value)
{
	Super::InputActionTriggered(Value);
	
	if (Value.Get<bool>() == false)
	{
		return;
	}
	
	if (TargetingSystemComponent && GetCombatSystemComponent())
	{
		AActor* TargetedActor = TargetingSystemComponent->GetTargetPoint().GetActor();
		ETeamAttitude::Type Attitude = UCrysBlueprintFunctionLibrary::GetAttitudeTowardsActor(ControlledPawn, TargetedActor);
		
		if (Attitude == ETeamAttitude::Hostile)
		{
			if (bCombatStance)
			{
				bWaitingForCombatStance = false;
				CombatSystemComponent->StartAutoAttack();
			}
			else if (GetAbilitySystemComponent())
			{
				bWaitingForCombatStance = true;
				AbilitySystemComponent->TryActivateAbilitiesByTag(Crys::NativeGameplayTag::Ability_Combat_CombatStance.GetTag().GetSingleTagContainer());
			}
			return;
		}
	}
	
	if (InteractorComponent)
	{
		InteractorComponent->BeginInteract();
	}
}

void UInputActionListener_AttackInteract::InputActionCompleted(const FInputActionValue& Value)
{
	Super::InputActionCompleted(Value);
	
	if (InteractorComponent)
	{
		InteractorComponent->EndInteract();
	}
}

void UInputActionListener_AttackInteract::InputActionCanceled(const FInputActionValue& Value)
{
	Super::InputActionCanceled(Value);
	
	if (InteractorComponent)
	{
		InteractorComponent->EndInteract();
	}
}

void UInputActionListener_AttackInteract::OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn)
{
	Super::OnPossessedPawnChanged(OldPawn, NewPawn);
	
	InteractorComponent = UInteractionSystemBlueprintFunctionLibrary::GetInteractorComponent(NewPawn);
	ControlledPawn = NewPawn;
}

UCombatSystemComponent* UInputActionListener_AttackInteract::GetCombatSystemComponent()
{
	if (!CombatSystemComponent)
	{
		CombatSystemComponent = UCombatBlueprintFunctionLibrary::GetCombatSystemComponent(GetPlayerController()->GetPlayerState<APlayerState>());
	}
	return CombatSystemComponent;
}

UCrimAbilitySystemComponent* UInputActionListener_AttackInteract::GetAbilitySystemComponent()
{
	if (!AbilitySystemComponent)
	{
		AbilitySystemComponent = UCrimAbilitySystemBlueprintFunctionLibrary::GetAbilitySystemComponent(GetPlayerController()->GetPlayerState<APlayerState>());
		
		if (AbilitySystemComponent)
		{
			AbilitySystemComponent->RegisterGameplayTagEvent(Crys::NativeGameplayTag::Ability_State_CombatStance, 
				EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UInputActionListener_AttackInteract::OnCombatStanceGameplayTagCountChanged);
			OnCombatStanceGameplayTagCountChanged(Crys::NativeGameplayTag::Ability_State_CombatStance, 
				AbilitySystemComponent->GetGameplayTagCount(Crys::NativeGameplayTag::Ability_State_CombatStance));
		}
	}
	
	return AbilitySystemComponent;
}

void UInputActionListener_AttackInteract::OnCombatStanceGameplayTagCountChanged(FGameplayTag Tag, int32 NewCount)
{
	if (NewCount > 0)
	{
		bCombatStance = true;
		
		if (bWaitingForCombatStance && GetCombatSystemComponent())
		{
			bWaitingForCombatStance = false;
			FTimerDelegate Delegate;
			Delegate.BindWeakLambda(this, [this]()
			{
				if (GetCombatSystemComponent())
				{
					CombatSystemComponent->StartAutoAttack();
				}
			});
			GetWorld()->GetTimerManager().SetTimerForNextTick(Delegate);
		}
	}
	else
	{
		bCombatStance = false;
	}
}

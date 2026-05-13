// Copyright Soccertitan 2025


#include "Input/InputActionListener/AttackInteractInputActionListener.h"

#include "CrimAbilitySystemBlueprintFunctionLibrary.h"
#include "CrimAbilitySystemComponent.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerState.h"

UAttackInteractInputActionListener::UAttackInteractInputActionListener()
{
}

void UAttackInteractInputActionListener::OnInitializeListener()
{
	Super::OnInitializeListener();
	
	// GetAutoAttackManagerComponent();
	GetAbilitySystemComponent();
}

void UAttackInteractInputActionListener::OnInputActionTriggered(const FInputActionValue& Value)
{
	Super::OnInputActionTriggered(Value);
	
	if (Value.Get<bool>() == false)
	{
		return;
	}
	
	// if (TargetingSystemComponent && GetAutoAttackManagerComponent())
	// {
	// 	AActor* TargetedActor = TargetingSystemComponent->GetTargetedActor();
	// 	ETeamAttitude::Type Attitude = UCrysBlueprintFunctionLibrary::GetAttitudeTowardsActor(ControlledPawn, TargetedActor);
	// 	
	// 	if (Attitude == ETeamAttitude::Hostile)
	// 	{
	// 		if (bCombatStance)
	// 		{
	// 			bWaitingForCombatStance = false;
	// 			AutoAttackManagerComponent->StartAutoAttack();
	// 		}
	// 		else if (GetAbilitySystemComponent())
	// 		{
	// 			bWaitingForCombatStance = true;
	// 			AbilitySystemComponent->TryActivateAbilitiesByTag(Crys::NativeGameplayTag::Ability_Combat_CombatStance.GetTag().GetSingleTagContainer());
	// 		}
	// 		return;
	// 	}
	// }
	//
	// if (InteractorComponent)
	// {
	// 	InteractorComponent->BeginInteract();
	// }
}

void UAttackInteractInputActionListener::OnInputActionCompleted(const FInputActionValue& Value)
{
	Super::OnInputActionCompleted(Value);
	
	// if (InteractorComponent)
	// {
	// 	// InteractorComponent->EndInteract();
	// }
}

void UAttackInteractInputActionListener::OnInputActionCanceled(const FInputActionValue& Value)
{
	Super::OnInputActionCanceled(Value);
	
	// if (InteractorComponent)
	// {
	// 	InteractorComponent->EndInteract();
	// }
}

void UAttackInteractInputActionListener::OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn)
{
	Super::OnPossessedPawnChanged(OldPawn, NewPawn);
	
	// TargetingSystemComponent = UTargetingSystemBlueprintFunctionLibrary::GetTargetingSystemComponent(NewPawn);
	// InteractorComponent = UInteractionSystemBlueprintFunctionLibrary::GetInteractorComponent(NewPawn);
	// ControlledPawn = NewPawn;
}

// UAutoAttackManagerComponent* UAttackInteractInputActionListener::GetAutoAttackManagerComponent()
// {
// 	if (!AutoAttackManagerComponent)
// 	{
// 		if (GetPlayerController() && GetPlayerController()->GetPlayerState<APlayerState>())
// 		{
// 			// AutoAttackManagerComponent = GetPlayerController()->GetPlayerState<APlayerState>()->FindComponentByClass<UAutoAttackManagerComponent>();
// 		}
// 	}
// 	
// 	return AutoAttackManagerComponent;
// }

UCrimAbilitySystemComponent* UAttackInteractInputActionListener::GetAbilitySystemComponent()
{
	if (!AbilitySystemComponent)
	{
		AbilitySystemComponent = UCrimAbilitySystemBlueprintFunctionLibrary::GetAbilitySystemComponent(GetPlayerController()->GetPlayerState<APlayerState>());
		
		// if (AbilitySystemComponent)
		// {
		// 	AbilitySystemComponent->RegisterGameplayTagEvent(Crys::NativeGameplayTag::Ability_State_CombatStance, 
		// 		EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UAttackInteractInputActionListener::OnCombatStanceGameplayTagCountChanged);
		// 	OnCombatStanceGameplayTagCountChanged(Crys::NativeGameplayTag::Ability_State_CombatStance, 
		// 		AbilitySystemComponent->GetGameplayTagCount(Crys::NativeGameplayTag::Ability_State_CombatStance));
		// }
	}
	
	return AbilitySystemComponent;
}

void UAttackInteractInputActionListener::OnCombatStanceGameplayTagCountChanged(FGameplayTag Tag, int32 NewCount)
{
	if (NewCount > 0)
	{
		bCombatStance = true;
		
		// if (bWaitingForCombatStance && GetAutoAttackManagerComponent())
		// {
		// 	bWaitingForCombatStance = false;
		// 	FTimerDelegate Delegate;
		// 	Delegate.BindWeakLambda(this, [this]()
		// 	{
		// 		if (GetAutoAttackManagerComponent())
		// 		{
		// 			// AutoAttackManagerComponent->StartAutoAttack();
		// 		}
		// 	});
		// 	GetWorld()->GetTimerManager().SetTimerForNextTick(Delegate);
		// }
	}
	else
	{
		bCombatStance = false;
	}
}

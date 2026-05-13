// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "CrysInputActionListener.h"
#include "GameplayTagContainer.h"
#include "AttackInteractInputActionListener.generated.h"

class UCrimAbilitySystemComponent;

/**
 * Starts the auto attack timer if the current target is an enemy. Otherwise, it will try to interact with objects.
 */
UCLASS()
class CRYSPROJECT_API UAttackInteractInputActionListener : public UCrysInputActionListener
{
	GENERATED_BODY()
	
public:
	UAttackInteractInputActionListener();
	
protected:
	virtual void OnInitializeListener() override;
	virtual void OnInputActionTriggered(const FInputActionValue& Value) override;
	virtual void OnInputActionCompleted(const FInputActionValue& Value) override;
	virtual void OnInputActionCanceled(const FInputActionValue& Value) override;
	
	virtual void OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn) override;
	
	// UAutoAttackManagerComponent* GetAutoAttackManagerComponent();
	UCrimAbilitySystemComponent* GetAbilitySystemComponent();
	
private:
	/** Cached from the controlled pawn. */
	// UPROPERTY()
	// TObjectPtr<UTargetingSystemComponent> TargetingSystemComponent;
	//
	// UPROPERTY()
	// TObjectPtr<UInteractorComponent> InteractorComponent;
	//
	// UPROPERTY()
	// TObjectPtr<UAutoAttackManagerComponent> AutoAttackManagerComponent;
	
	UPROPERTY()
	TObjectPtr<UCrimAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY()
	TObjectPtr<APawn> ControlledPawn;
	
	void OnCombatStanceGameplayTagCountChanged(FGameplayTag Tag, int32 NewCount);
	
	bool bCombatStance = false;
	bool bWaitingForCombatStance = false;
};

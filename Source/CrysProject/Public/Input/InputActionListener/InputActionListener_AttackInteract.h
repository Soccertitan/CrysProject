// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "CrysInputActionListener.h"
#include "GameplayTagContainer.h"
#include "InputActionListener_AttackInteract.generated.h"

class UCrimTargetingSystemComponent;
class UInteractorComponent;
class UCombatSystemComponent;
class UCrimAbilitySystemComponent;

/**
 * Starts the auto attack timer if the current target is an enemy. Otherwise, it will try to interact with objects.
 */
UCLASS()
class CRYSPROJECT_API UInputActionListener_AttackInteract : public UCrysInputActionListener
{
	GENERATED_BODY()
	
public:
	UInputActionListener_AttackInteract();
	
protected:
	virtual void OnInitializeListener() override;
	virtual void OnInputActionTriggered(const FInputActionValue& Value) override;
	virtual void OnInputActionCompleted(const FInputActionValue& Value) override;
	virtual void OnInputActionCanceled(const FInputActionValue& Value) override;
	
	virtual void OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn) override;
	
	UCombatSystemComponent* GetCombatSystemComponent();
	UCrimAbilitySystemComponent* GetAbilitySystemComponent();
	
private:
	UPROPERTY()
	TObjectPtr<UCrimTargetingSystemComponent> TargetingSystemComponent;
	
	UPROPERTY()
	TObjectPtr<UInteractorComponent> InteractorComponent;
	
	UPROPERTY()
	TObjectPtr<UCombatSystemComponent> CombatSystemComponent;
	
	UPROPERTY()
	TObjectPtr<UCrimAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY()
	TObjectPtr<APawn> ControlledPawn;
	
	void OnCombatStanceGameplayTagCountChanged(FGameplayTag Tag, int32 NewCount);
	
	bool bCombatStance = false;
	bool bWaitingForCombatStance = false;
};

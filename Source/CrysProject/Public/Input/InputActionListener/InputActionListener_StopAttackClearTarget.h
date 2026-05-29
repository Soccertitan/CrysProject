// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "Input/InputActionListener/CrysInputActionListener.h"
#include "InputActionListener_StopAttackClearTarget.generated.h"


class UCrimTargetingSystemComponent;
class UCombatSystemComponent;

/**
 * If AutoAttacking, stop auto attacking. Else if targeting an actor, clear the target.
 */
UCLASS()
class CRYSPROJECT_API UInputActionListener_StopAttackClearTarget : public UCrysInputActionListener
{
	GENERATED_BODY()
	
public:
	virtual void InputActionTriggered(const FInputActionValue& Value) override;
	
protected:
	virtual void Initialize() override;
	virtual void OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn) override;
	
	UCombatSystemComponent* GetCombatSystem();
	
private:
	UPROPERTY()
	TObjectPtr<UCombatSystemComponent> CombatSystemComponent;
	
	UPROPERTY()
	TObjectPtr<UCrimTargetingSystemComponent> TargetingSystemComponent;
};

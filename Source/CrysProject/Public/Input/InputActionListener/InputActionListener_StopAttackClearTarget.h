// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "Input/InputActionListener/CrysInputActionListener.h"
#include "InputActionListener_StopAttackClearTarget.generated.h"


/**
 * If AutoAttacking, stop auto attacking. Else if targeting an actor, clear the target.
 */
UCLASS()
class CRYSPROJECT_API UInputActionListener_StopAttackClearTarget : public UCrysInputActionListener
{
	GENERATED_BODY()
	
public:
	
protected:
	virtual void OnInitializeListener() override;
	virtual void OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn) override;
	
	virtual void OnInputActionTriggered(const FInputActionValue& Value) override;
	
	// UAutoAttackManagerComponent* GetAutoAttackManagerComponent();
	
private:
	// UPROPERTY()
	// TObjectPtr<UAutoAttackManagerComponent> AutoAttackManagerComponent;
	
	// UPROPERTY()
	// TObjectPtr<UTargetingSystemComponent> TargetingSystemComponent;
};

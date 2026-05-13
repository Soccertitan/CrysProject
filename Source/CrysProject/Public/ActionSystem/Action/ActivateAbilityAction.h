// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "ActionSystem/CrysAction.h"
#include "ActivateAbilityAction.generated.h"

class UCrimGameplayAbility;
class UAbilityInputManagerComponent;

/**
 * Tries to activate a gameplay ability from the PlayerControllers AbilityInputComponent.
 */
UCLASS(Abstract)
class CRYSPROJECT_API UActivateAbilityAction : public UCrysAction
{
	GENERATED_BODY()
	
public:
	virtual bool CanActivateAction() const override;
	
protected:
	virtual void InitializeAction() override;
	virtual void ActivateAction_Implementation() override;
	
private:
	UPROPERTY()
	TObjectPtr<UAbilityInputManagerComponent> AbilityInputComponent;
	
	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<UCrimGameplayAbility> AbilityClass;
};

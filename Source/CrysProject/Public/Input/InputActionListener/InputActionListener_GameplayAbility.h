// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "CrysInputActionListener.h"
#include "InputActionListener_GameplayAbility.generated.h"

class UGameplayAbility;
class UAbilityInputManagerComponent;

/**
 * Activates a gameplay ability by AbilityClass.
 */
UCLASS(Abstract)
class CRYSPROJECT_API UInputActionListener_GameplayAbility : public UCrysInputActionListener
{
	GENERATED_BODY()
	
public:
	virtual void InputActionTriggered(const FInputActionValue& Value) override;
	virtual void InputActionCanceled(const FInputActionValue& Value) override;
	virtual void InputActionCompleted(const FInputActionValue& Value) override;
	
protected:
	virtual void Initialize() override;
	
private:
	UPROPERTY()
	TObjectPtr<UAbilityInputManagerComponent> AbilityInputManagerComponent;
	
	UPROPERTY(EditAnywhere)
	TSoftClassPtr<UGameplayAbility> AbilityClass;
};

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
	
protected:
	virtual void OnInitializeListener() override;
	virtual void OnInputActionTriggered(const FInputActionValue& Value) override;
	virtual void OnInputActionCanceled(const FInputActionValue& Value) override;
	virtual void OnInputActionCompleted(const FInputActionValue& Value) override;
	
private:
	UPROPERTY()
	TObjectPtr<UAbilityInputManagerComponent> AbilityInputManagerComponent;
	
	UPROPERTY(EditAnywhere)
	TSoftClassPtr<UGameplayAbility> AbilityClass;
};

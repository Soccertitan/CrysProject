// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "CrysInputActionListener.h"
#include "InputActionListener_CycleHotbar.generated.h"

class UAbilityInputManagerComponent;

/**
 * Switches a hotbar to the next or previous one. Cycles through.
 */
UCLASS()
class CRYSPROJECT_API UInputActionListener_CycleHotbar : public UCrysInputActionListener
{
	GENERATED_BODY()
	
public:
	virtual void InputActionTriggered(const FInputActionValue& Value) override;
	
protected:
	virtual void Initialize() override;
	
private:
	UPROPERTY()
	TObjectPtr<UAbilityInputManagerComponent> AbilityInputManagerComponent;
};

// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "CrysInputActionListener.h"
#include "Input/AbilityInputSlot.h"
#include "InputActionListener_GameplayAbility.generated.h"

class UAbilityInput;
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
	
#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
protected:
	virtual void Initialize() override;
	
private:
	UPROPERTY()
	TObjectPtr<UAbilityInputManagerComponent> AbilityInputManagerComponent;
	
	UPROPERTY(EditAnywhere, Category = "Ability")
	bool bUseInputSlot = true;
	
	UPROPERTY(EditAnywhere, Category = "Ability|Slot", meta = (EditCondition = "bUseInputSlot", ShowOnlyInnerProperties))
	FAbilityInputSlot InputSlot;
	
	UPROPERTY(EditAnywhere, Category = "Ability|Slot", meta = (EditCondition = "bUseInputSlot", ClampMin = 0))
	int32 InputSet = 0;
	
	UPROPERTY(EditAnywhere, Category = "Ability", meta = (EditCondition = "bUseInputSlot==false"))
	TObjectPtr<UAbilityInput> AbilityInput;
	
	void InternalAbilityInputPressed();
	void InternalAbilityInputReleased();
};

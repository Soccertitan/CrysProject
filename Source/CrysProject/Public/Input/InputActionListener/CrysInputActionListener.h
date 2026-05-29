// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "InputActionListener.h"
#include "CrysInputActionListener.generated.h"

class ACrysPlayerController;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCrysInputActionListenerGenericSignature, UCrysInputActionListener*, Action);

/**
 * A generic action that broadcasts when an input is pressed/released.
 */
UCLASS()
class CRYSPROJECT_API UCrysInputActionListener : public UInputActionListener
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable)
	FCrysInputActionListenerGenericSignature OnInputPressed;
	UPROPERTY(BlueprintAssignable)
	FCrysInputActionListenerGenericSignature OnInputReleased;
	
	UFUNCTION(BlueprintPure, Category = "InputListener")
	bool IsPressed() const { return bPressed; }
	
	virtual void InputActionTriggered(const FInputActionValue& Value) override;
	virtual void InputActionCanceled(const FInputActionValue& Value) override;
	virtual void InputActionCompleted(const FInputActionValue& Value) override;

protected:
	virtual void Initialize() override;
	
	UFUNCTION()
	virtual void OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn);
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "OnPossedPawnChanged")
	void K2_OnPossesedPawnChanged(APawn* OldPawn, APawn* NewPawn);
	
private:
	UPROPERTY()
	bool bPressed = false;
};

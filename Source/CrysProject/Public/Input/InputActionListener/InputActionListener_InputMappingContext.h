// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "CrysInputActionListener.h"
#include "EnhancedInputSubsystemInterface.h"
#include "InputActionListener_InputMappingContext.generated.h"

class UEnhancedInputLocalPlayerSubsystem;
class UInputMappingContext;

/**
 * Adds/Removes an Input Mapping Context when Triggered/Completed/Canceled.
 */
UCLASS(Abstract)
class CRYSPROJECT_API UInputActionListener_InputMappingContext : public UCrysInputActionListener
{
	GENERATED_BODY()
	
protected:
	virtual void OnInitializeListener() override;

	virtual void OnInputActionTriggered(const FInputActionValue& Value) override;
	virtual void OnInputActionCompleted(const FInputActionValue& Value) override;
	virtual void OnInputActionCanceled(const FInputActionValue& Value) override;
	
private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputMappingContext> InputMappingContext;
	UPROPERTY(EditAnywhere)
	int32 Priority = 1;
	UPROPERTY(EditAnywhere)
	FModifyContextOptions ContextOptionsOnAdd;
	UPROPERTY(EditAnywhere)
	FModifyContextOptions ContextOptionsOnRemove;

	UPROPERTY()
	TObjectPtr<UEnhancedInputLocalPlayerSubsystem> EnhancedInputSubsystem;

	bool bAppliedContext = false;
};

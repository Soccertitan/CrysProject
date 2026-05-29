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
	
public:
	virtual void InputActionTriggered(const FInputActionValue& Value) override;
	virtual void InputActionCompleted(const FInputActionValue& Value) override;
	virtual void InputActionCanceled(const FInputActionValue& Value) override;
	
protected:
	virtual void Initialize() override;
	
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

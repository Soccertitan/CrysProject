// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "CrysInputActionListener.h"
#include "InputActionListener_CycleTarget.generated.h"

struct FTargetingRequestHandle;
class UCrimTargetingSystemComponent;
class UTargetingPreset;
class UCrysTargetingContext;
/**
 * Targets an actor using the TargetingSubsystem.
 */
UCLASS()
class CRYSPROJECT_API UInputActionListener_CycleTarget : public UCrysInputActionListener
{
	GENERATED_BODY()
	
public:
	UInputActionListener_CycleTarget();
	
protected:
	virtual void OnInitializeListener() override;
	virtual void OnInputActionTriggered(const FInputActionValue& Value) override;
	
	UFUNCTION()
	void OnTargetingRequestCompleted(FTargetingRequestHandle Handle);
	
private:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UTargetingPreset> TargetingPreset;
	UPROPERTY()
	TObjectPtr<UCrysTargetingContext> TargetingContext;
	
	UPROPERTY()
	TObjectPtr<UCrimTargetingSystemComponent> TargetingSystemComponent;
};

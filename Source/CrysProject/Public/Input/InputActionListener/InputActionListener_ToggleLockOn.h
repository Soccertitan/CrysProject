// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "CrysInputActionListener.h"
#include "InputActionListener_ToggleLockOn.generated.h"

class UCrimTargetingSystemComponent;

/**
 * Locks on or off from the targeting point.
 */
UCLASS()
class CRYSPROJECT_API UInputActionListener_ToggleLockOn : public UCrysInputActionListener
{
	GENERATED_BODY()
	
public:
	
protected:
	virtual void OnInitializeListener() override;
	
	virtual void OnInputActionTriggered(const FInputActionValue& Value) override;
	
private:
	UPROPERTY()
	TObjectPtr<UCrimTargetingSystemComponent> TargetingSystemComponent;
};

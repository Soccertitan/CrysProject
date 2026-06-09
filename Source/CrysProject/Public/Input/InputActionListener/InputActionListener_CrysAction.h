// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Input/InputActionListener/CrysInputActionListener.h"
#include "InputActionListener_CrysAction.generated.h"

class UCrysActionManagerComponent;
DECLARE_MULTICAST_DELEGATE_OneParam(FCrysActionActionSetIndexSignature, int32);

/**
 * Activates the action from the ActiveActionSet index of the ActionManager.
 */
UCLASS(Abstract)
class CRYSPROJECT_API UInputActionListener_CrysAction : public UCrysInputActionListener
{
	GENERATED_BODY()

public:
	virtual void InputActionTriggered(const FInputActionValue& Value) override;
	
protected:
	virtual void Initialize() override;
	
private:
	/** The action "slot" to activate on the ActionBar. */
	UPROPERTY(EditAnywhere, meta = (Categories="Input"))
	FGameplayTag InputTag;
	
	/** The slot to activate if InputTag is invalid. */
	UPROPERTY(EditAnywhere, meta = (ClampMin = -1))
	int32 ActionIndex = -1;
	
	UPROPERTY()
	TObjectPtr<UCrysActionManagerComponent> ActionManager;
};

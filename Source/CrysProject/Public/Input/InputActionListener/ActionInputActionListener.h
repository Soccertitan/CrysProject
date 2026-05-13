// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Input/InputActionListener/CrysInputActionListener.h"
#include "ActionInputActionListener.generated.h"

class UCrysActionManagerComponent;
DECLARE_MULTICAST_DELEGATE_OneParam(FCrysActionActionSetIndexSignature, int32);

/**
 * Activates the action in the currently selected index of the ActionManager.
 */
UCLASS(Abstract)
class CRYSPROJECT_API UActionInputActionListener : public UCrysInputActionListener
{
	GENERATED_BODY()
	
protected:
	virtual void OnInitializeListener() override;
	
	virtual void OnInputActionTriggered(const FInputActionValue& Value) override;
	
private:
	/** The action "slot" to activate on the ActionBar. */
	UPROPERTY(EditAnywhere, meta = (Categories="Input"))
	FGameplayTag InputTag;
	
	UPROPERTY()
	TObjectPtr<UCrysActionManagerComponent> ActionManager;
};

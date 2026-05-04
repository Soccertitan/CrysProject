// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "CrysCheckPoint.generated.h"

UCLASS(Abstract)
class CRYSPROJECT_API ACrysCheckPoint : public APlayerStart
{
	GENERATED_BODY()

public:
	ACrysCheckPoint(const FObjectInitializer& ObjectInitializer);

	/** Updates the checkpoint on the GameInstance. */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "CheckPoint")
	void TriggerCheckPoint();

protected:

	/** Called [Server + Client] anytime the checkpoint is triggered. */
	UFUNCTION(BlueprintImplementableEvent, Category = "CheckPoint")
	void OnCheckPointTriggered();

private:
	UFUNCTION(NetMulticast, Unreliable)
	void Multi_OnCheckPointTriggered();
};

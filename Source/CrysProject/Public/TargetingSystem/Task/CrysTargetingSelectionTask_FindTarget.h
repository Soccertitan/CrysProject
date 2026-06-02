// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "TargetingTask/TargetingSelectionTask_FindTarget.h"
#include "CrysTargetingSelectionTask_FindTarget.generated.h"

/**
 * Gets the CrysTargetingContext to determine the search direction.
 */
UCLASS()
class CRYSPROJECT_API UCrysTargetingSelectionTask_FindTarget : public UTargetingSelectionTask_FindTarget
{
	GENERATED_BODY()
	
protected:
	virtual FVector2D GetSearchDirection(const FTargetingRequestHandle& TargetingHandle) const override;
};

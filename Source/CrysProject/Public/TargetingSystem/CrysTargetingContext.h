// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "CrimTargetingTypes.h"
#include "UObject/Object.h"
#include "CrysTargetingContext.generated.h"

/**
 * The basic context used for Targeting tasks in this project.
 */
UCLASS(DefaultToInstanced)
class CRYSPROJECT_API UCrysTargetingContext : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, meta = (ValidEnumValues="Left,Right,Up,Down"))
	EUINavigation SearchDirection;
	
	UPROPERTY()
	FCrimTargetPoint StartingTargetPoint;
};

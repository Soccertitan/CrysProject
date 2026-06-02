// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/AbilityTargetType.h"
#include "UObject/Object.h"
#include "CrysTargetingContext.generated.h"

enum class ETargetingSearchDirection : uint8;

/**
 * The basic context used for Targeting tasks in this project.
 */
UCLASS(DefaultToInstanced)
class CRYSPROJECT_API UCrysTargetingContext : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D SearchDirection;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAbilityTargetType AbilityTargetType;
};

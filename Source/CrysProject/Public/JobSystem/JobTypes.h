// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "ScalableFloat.h"
#include "JobTypes.generated.h"

class UJobDefinition;

/** The base attributes to grant the character. */
USTRUCT()
struct FPrimaryAttributes
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FScalableFloat MaxHitPoints;

	UPROPERTY(EditAnywhere)
	FScalableFloat MaxMagicPoints;

	UPROPERTY(EditAnywhere)
	FScalableFloat Strength;

	UPROPERTY(EditAnywhere)
	FScalableFloat Vitality;

	UPROPERTY(EditAnywhere)
	FScalableFloat Dexterity;
	
	UPROPERTY(EditAnywhere)
	FScalableFloat Agility;

	UPROPERTY(EditAnywhere)
	FScalableFloat Intelligence;

	UPROPERTY(EditAnywhere)
	FScalableFloat Mind;

	UPROPERTY(EditAnywhere)
	FScalableFloat Charisma;
};

/** Calculates the attributes using Race, MainJob, and SubJob. */
USTRUCT(BlueprintType)
struct FPrimaryAttributesCalc
{
	GENERATED_BODY()

	FPrimaryAttributesCalc() {}
	FPrimaryAttributesCalc(
		const UJobDefinition* Race, const int32 Level, 
		const UJobDefinition* MainJob, const int32 MainJobLevel, 
		const UJobDefinition* SubJob, const int32 SubJobLevel, const float SubJobEfficiency);
	
	static int32 CalculateValue(const float Race, const float MainJobMultiplier, const float SubJobMultiplier, const float SubJobEfficiency);

	UPROPERTY(BlueprintReadOnly)
	int32 MaxHitPoints = 1;
	UPROPERTY(BlueprintReadOnly)
	int32 MaxMagicPoints = 1;
	UPROPERTY(BlueprintReadOnly)
	int32 Strength = 1;
	UPROPERTY(BlueprintReadOnly)
	int32 Vitality = 1;
	UPROPERTY(BlueprintReadOnly)
	int32 Dexterity = 1;
	UPROPERTY(BlueprintReadOnly)
	int32 Agility = 1;
	UPROPERTY(BlueprintReadOnly)
	int32 Intelligence = 1;
	UPROPERTY(BlueprintReadOnly)
	int32 Mind = 1;
	UPROPERTY(BlueprintReadOnly)
	int32 Charisma = 1;
};

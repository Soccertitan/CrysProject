// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameplayTagContainer.h"
#include "ScalableFloat.h"

#include "ExecCalcTypes.generated.h"

/** Used by an ExecCalc to determine if a calculation can occur and the limitations. */
USTRUCT()
struct FExecCalcEffectChance
{
	GENERATED_BODY()
	
	/** The minimum chance an effect can occur. */
	UPROPERTY(EditAnywhere, meta = (ClampMin = 0.f, ClampMax = 1.f))
	FScalableFloat MinChance = 0.f;
	
	/** The maximum chance an effect can occur. */
	UPROPERTY(EditAnywhere, meta = (ClampMin = 0.f, ClampMax = 1.f))
	FScalableFloat MaxChance = 1.f;
	
	/** These requirements must be met in order to roll for success. */
	UPROPERTY(EditAnywhere)
	FGameplayTagRequirements SourceTagRequirements;
	
	/** These requirements must be met in order to roll for success. */
	UPROPERTY(EditAnywhere)
	FGameplayTagRequirements TargetTagRequirements;
	
	/** If these tags requirements are met, the result will always be true. */
	UPROPERTY(EditAnywhere)
	FGameplayTagRequirements AutoSuccessTagRequirements;
	
	/** If these tags requirements met, the result will always be false. */
	UPROPERTY(EditAnywhere)
	FGameplayTagRequirements AutoFailTagRequirements;
	
	/** If false, AutoFailTagRequirements will have priority. */
	UPROPERTY(EditAnywhere)
	bool bAutoSuccessTagsHasPriority = true;
	
	/** 
	 * If true, the SourceTags are used for AutoSuccessRequirements and TargetTags are used for AutoFailRequirements.
	 * If false, the inverse is done.
	 */
	UPROPERTY(EditAnywhere)
	bool bUseSourceTagsForAutoSuccess = true;
	
	/** 
	 * If the Source and Target Tag requirements are met. Rolls a random number between 0.f and 1.f.
	 * @param Chance The likelihood of success, clamped between Min and Max chance.
	 * @param Level The value to evaluate the Min/Max Chance Scalable float.
	 * @param SourceTags Tags from source.
	 * @param TargetTags Tags from target.
	 */
	bool CalculateIsSuccess(float Chance, const float Level, const FGameplayTagContainer& SourceTags, const FGameplayTagContainer& TargetTags) const;
};

USTRUCT(BlueprintType)
struct FAbilityDamageContext
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly)
	bool bHit = false;
	
	UPROPERTY(BlueprintReadOnly)
	bool bCriticalHit = false;
	
	UPROPERTY(BlueprintReadOnly)
	bool bGuarded = false;
	
	UPROPERTY(BlueprintReadOnly)
	bool bParried = false;
	
	UPROPERTY(BlueprintReadOnly)
	bool bBlocked = false;
	
	UPROPERTY(BlueprintReadOnly)
	FGameplayAttribute DamagedAttribute;
};

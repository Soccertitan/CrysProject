// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/AbilityTargetType.h"
#include "Tasks/TargetingFilterTask_BasicFilterTemplate.h"
#include "TargetingFilterTask_AbilityTargetType.generated.h"

/**
 * Filters out targets that don't meet the target type.
 */
UCLASS()
class CRYSPROJECT_API UTargetingFilterTask_AbilityTargetType : public UTargetingFilterTask_BasicFilterTemplate
{
	GENERATED_BODY()
	
public:
	
protected:
	/** Use this target type unless the context specifies otherwise. */
	UPROPERTY(EditAnywhere, Category = "Targeting Filter")
	EAbilityTargetType AbilityTargetType;
	
	virtual bool ShouldFilterTarget(const FTargetingRequestHandle& TargetingHandle, const FTargetingDefaultResultData& TargetData) const override;
};

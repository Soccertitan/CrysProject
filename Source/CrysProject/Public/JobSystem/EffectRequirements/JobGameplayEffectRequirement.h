// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "JobGameplayEffectRequirement.generated.h"

class UJobSystemComponent;

/**
 * Defines custom requirements to grant gameplay effects for a job.
 */
UCLASS(Abstract, Blueprintable, BlueprintType, DefaultToInstanced)
class CRYSPROJECT_API UJobGameplayEffectRequirement : public UObject
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintNativeEvent)
	bool ShouldGrantEffect(const UJobSystemComponent* JobSystemComponent) const;
};

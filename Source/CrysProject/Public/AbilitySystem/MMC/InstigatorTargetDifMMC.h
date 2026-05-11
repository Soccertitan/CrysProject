// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "CrysGameplayMMC.h"
#include "InstigatorTargetDifMMC.generated.h"

/**
 * Takes the difference between the Instigator's and Target's chosen attributes. (i.e Instigator STR - Target VIT)
 */
UCLASS()
class CRYSPROJECT_API UInstigatorTargetDifMMC : public UCrysGameplayMMC
{
	GENERATED_BODY()
	
public:
	UInstigatorTargetDifMMC();
	
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes")
	FGameplayEffectAttributeCaptureDefinition InstigatorAttribute;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes")
	FGameplayEffectAttributeCaptureDefinition TargetAttribute;
	
	virtual void UpdateAggregatedRelevantAttributesToCapture() override;
};

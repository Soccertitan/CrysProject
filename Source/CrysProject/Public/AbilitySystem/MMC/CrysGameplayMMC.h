// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "CrysGameplayMMC.generated.h"

/**
 * 
 */
UCLASS()
class CRYSPROJECT_API UCrysGameplayMMC : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
	
public:
	UCrysGameplayMMC();
	virtual const TArray<FGameplayEffectAttributeCaptureDefinition>& GetAttributeCaptureDefinitions() const override;
	
#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif WITH_EDITOR
	
protected:
	/**
	 * Gets the captured magnitude value for the given Attribute
	 * For this to work correctly, the Attribute needs to be added to the Relevant Attributes to Capture array
	 *
	 * @param EffectSpec The Gameplay Effect Spec to get the info from
	 * @param Definition The attribute capture definition
	 *
	 * @return The magnitude value if found, zero otherwise
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Ability|GameplayEffect", meta=(DisplayName="Get Captured Attribute Magnitude", ScriptName="GetCapturedAttributeMagnitude"))
	float K2_GetCapturedAttributeMagnitudeCrys(const FGameplayEffectSpec& EffectSpec, FGameplayEffectAttributeCaptureDefinition Definition, const FGameplayTagContainer& SourceTags, const FGameplayTagContainer& TargetTags) const;
	
	//** Combines the Relevant Attributes to Capture with the other attributes defined in this class. */
	UPROPERTY()
	TArray<FGameplayEffectAttributeCaptureDefinition> AggregatedRelevantAttributesToCapture;
	
	/** Called in the constructor and PostEditChangeProperty. */
	virtual void UpdateAggregatedRelevantAttributesToCapture();
};

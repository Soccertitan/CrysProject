// Copyright Soccertitan 2025


#include "AbilitySystem/MMC/CrysGameplayMMC.h"

UCrysGameplayMMC::UCrysGameplayMMC()
{
}

const TArray<FGameplayEffectAttributeCaptureDefinition>& UCrysGameplayMMC::GetAttributeCaptureDefinitions() const
{
	return AggregatedRelevantAttributesToCapture;
}

void UCrysGameplayMMC::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	
	UpdateAggregatedRelevantAttributesToCapture();
}

void UCrysGameplayMMC::UpdateAggregatedRelevantAttributesToCapture()
{
	AggregatedRelevantAttributesToCapture.Empty();
	
	AggregatedRelevantAttributesToCapture.Append(RelevantAttributesToCapture);
}

float UCrysGameplayMMC::K2_GetCapturedAttributeMagnitudeCrys(const FGameplayEffectSpec& EffectSpec,
	FGameplayEffectAttributeCaptureDefinition Definition, const FGameplayTagContainer& SourceTags,
    const FGameplayTagContainer& TargetTags) const
{
	float Magnitude = 0.0f;
	
	// configure the aggregator evaluation parameters
	// TODO: apply filters?
	FAggregatorEvaluateParameters EvaluationParameters;

	EvaluationParameters.SourceTags = &SourceTags;
	EvaluationParameters.TargetTags = &TargetTags;

	// get the attribute magnitude
	GetCapturedAttributeMagnitude(Definition, EffectSpec, EvaluationParameters, Magnitude);

	return Magnitude;
}

// Copyright Soccertitan 2025


#include "AbilitySystem/MMC/InstigatorTargetDifMMC.h"

#include "AbilitySystem/AttributeSet/PrimaryAttributeSet.h"

UInstigatorTargetDifMMC::UInstigatorTargetDifMMC()
{
	InstigatorAttribute.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	InstigatorAttribute.AttributeToCapture = UPrimaryAttributeSet::GetStrengthAttribute();
	
	TargetAttribute.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	TargetAttribute.AttributeToCapture = UPrimaryAttributeSet::GetVitalityAttribute();
	
	UpdateAggregatedRelevantAttributesToCapture();
}

float UInstigatorTargetDifMMC::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	FAggregatorEvaluateParameters EvaluateParams;
	EvaluateParams.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvaluateParams.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	
	float InstigatorAttributeValue = 0.f;
	GetCapturedAttributeMagnitude(InstigatorAttribute, Spec, EvaluateParams, InstigatorAttributeValue);
	float TargetAttributeValue = 0.f;
	GetCapturedAttributeMagnitude(TargetAttribute, Spec, EvaluateParams, TargetAttributeValue);
	
	return InstigatorAttributeValue - TargetAttributeValue;
}

void UInstigatorTargetDifMMC::UpdateAggregatedRelevantAttributesToCapture()
{
	Super::UpdateAggregatedRelevantAttributesToCapture();
	
	AggregatedRelevantAttributesToCapture.Add(InstigatorAttribute);
	AggregatedRelevantAttributesToCapture.Add(TargetAttribute);
}

// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "CrysGameplayMMC.h"
#include "HitChanceMMC.generated.h"

/**
 * Designed to modify hit chance based on the Accuracy/Evasion stats of Attacker/Defender and level.
 */
UCLASS()
class CRYSPROJECT_API UHitChanceMMC : public UCrysGameplayMMC
{
	GENERATED_BODY()
	
public:
	UHitChanceMMC();
	
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes")
	FGameplayEffectAttributeCaptureDefinition AttackerLevelAttributeDef;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes")
	FGameplayEffectAttributeCaptureDefinition DefenderLevelAttributeDef;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes")
	FGameplayEffectAttributeCaptureDefinition AccuracyAttributeDef;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes")
	FGameplayEffectAttributeCaptureDefinition EvasionAttributeDef;
	
	/** Modifies the accuracy of the attacker based on the level difference. (AttackerLevel - DefenderLevel) * LevelModifierAccuracy */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Calculation")
	FScalableFloat LevelModifierAccuracy = 4.f;
	/** (Accuracy - Evasion) * AccuracyEvasionHitChanceMultiplier */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Calculation")
	FScalableFloat AccuracyEvasionHitChanceMultiplier = 0.005f;
	/** If the source has any of these tags, ignore the evasion attribute on the target. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Calculation", meta = (Categories = "Ability.State.Ignore.Evasion"))
	FGameplayTagContainer IgnoreEvasionAttributeTagContainer;
	
	virtual void UpdateAggregatedRelevantAttributesToCapture() override;
};

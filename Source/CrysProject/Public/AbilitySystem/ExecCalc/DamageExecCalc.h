// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "ExecCalcTypes.h"
#include "GameplayEffectExecutionCalculation.h"
#include "DamageExecCalc.generated.h"

struct FDamageGameplayContext;

/**
 * 
 */
UCLASS()
class CRYSPROJECT_API UDamageExecCalc : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	
public:
	UDamageExecCalc();
	
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
	virtual const TArray<FGameplayEffectAttributeCaptureDefinition>& GetAttributeCaptureDefinitions() const override;
	
#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif WITH_EDITOR
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Calculation")
	FGameplayEffectAttributeCaptureDefinition BaseDamageAttributeDef;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Calculation")
	FGameplayEffectAttributeCaptureDefinition HitChanceAttributeDef;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Calculation")
	FGameplayEffectAttributeCaptureDefinition CriticalHitChanceAttributeDef;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Calculation")
	FGameplayEffectAttributeCaptureDefinition CriticalHitBonusAttributeDef;
	
	/** The attribute damage will be applied to. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Calculation")
	FGameplayEffectAttributeCaptureDefinition IncomingDamageAttributeDef;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Calculation|Attacker")
	FGameplayEffectAttributeCaptureDefinition AttackAttributeDef;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Calculation|Attacker")
	FGameplayEffectAttributeCaptureDefinition AttackDefenseRatioCapAttributeDef;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Calculation|Attacker")
	FGameplayEffectAttributeCaptureDefinition DefencePierceAttributeDef;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Calculation|Attacker")
	FGameplayEffectAttributeCaptureDefinition DamageMultiplierAttributeDef;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Calculation|Defender")
	FGameplayEffectAttributeCaptureDefinition DefenceAttributeDef;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Calculation|Defender")
	FGameplayEffectAttributeCaptureDefinition ResistanceAttributeDef;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Calculation|Defender|Guard")
	FGameplayEffectAttributeCaptureDefinition GuardChanceAttributeDef;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Calculation|Defender|Guard")
	FGameplayEffectAttributeCaptureDefinition GuardHalfAngleAttributeDef;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Calculation|Defender|Guard")
	FGameplayEffectAttributeCaptureDefinition GuardDamageReductionAttributeDef;
	UPROPERTY(EditDefaultsOnly, Category = "Calculation|Defender|Guard")
	FExecCalcEffectChance GuardEffectChance;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Calculation|Defender|Parry")
	FGameplayEffectAttributeCaptureDefinition ParryChanceAttributeDef;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Calculation|Defender|Parry")
	FGameplayEffectAttributeCaptureDefinition ParryHalfAngleAttributeDef;
	UPROPERTY(EditDefaultsOnly, Category = "Calculation|Defender|Parry")
	FExecCalcEffectChance ParryEffectChance;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Calculation|Defender|Shield")
	FGameplayEffectAttributeCaptureDefinition BlockChanceAttributeDef;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Calculation|Defender|Shield")
	FGameplayEffectAttributeCaptureDefinition BlockHalfAngleAttributeDef;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Calculation|Defender|Shield")
	FGameplayEffectAttributeCaptureDefinition BlockDamageReductionAttributeDef;
	UPROPERTY(EditDefaultsOnly, Category = "Calculation|Defender|Shield")
	FExecCalcEffectChance BlockEffectChance;
	
	/** Rolls a random number between min and max and is multiplied to damage. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Calculation")
	FScalableFloat DamageRandomizerMin = 1.f;
	/** Rolls a random number between min and max and is multiplied to damage. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Calculation")
	FScalableFloat DamageRandomizerMax = 1.05f;
	
	/** The SourceTags are used for the SuccessRequirements. */
	UPROPERTY(EditDefaultsOnly, Category = "Calculation")
	FExecCalcEffectChance HitEffectChance;
	/** The SourceTags are used for the SuccessRequirements. */
	UPROPERTY(EditDefaultsOnly, Category = "Calculation")
	FExecCalcEffectChance CriticalHitEffectChance;

	//** Combines the Relevant Attributes to Capture with the other attributes defined in this class. */
	UPROPERTY()
	TArray<FGameplayEffectAttributeCaptureDefinition> AggregatedRelevantAttributesToCapture;
	
	/** Called in the constructor and PostEditChangeProperty. */
	virtual void UpdateAggregatedRelevantAttributesToCapture();
	
	virtual float CalculateDamage(const float BaseDamage, FAbilityDamageContext* AbilityDamageContext, const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput, const FAggregatorEvaluateParameters& EvaluateParams) const;
	
	bool IsHit(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FAggregatorEvaluateParameters& EvaluateParams) const;
	bool IsCriticalHit(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FAggregatorEvaluateParameters& EvaluateParams) const;
	bool IsParried(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FAggregatorEvaluateParameters& EvaluateParams) const;
	bool IsGuarded(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FAggregatorEvaluateParameters& EvaluateParams) const;
	bool IsBlocked(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FAggregatorEvaluateParameters& EvaluateParams) const;
};

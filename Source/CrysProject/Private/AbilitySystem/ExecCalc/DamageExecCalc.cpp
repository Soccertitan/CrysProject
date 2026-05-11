// Copyright Soccertitan 2025


#include "AbilitySystem/ExecCalc/DamageExecCalc.h"

#include "CrimGameplayEffectContext.h"
#include "CrimMathStatics.h"
#include "CrysNativeGameplayTags.h"
#include "AbilitySystem/ExecCalc/ExecCalcTypes.h"
#include "AbilitySystem/AttributeSet/AbilityAttributeSet.h"
#include "AbilitySystem/AttributeSet/AttackerAttributeSet.h"
#include "AbilitySystem/AttributeSet/CrysHitPointsAttributeSet.h"
#include "AbilitySystem/AttributeSet/DefenderAttributeSet.h"


UDamageExecCalc::UDamageExecCalc()
{
	BaseDamageAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	BaseDamageAttributeDef.AttributeToCapture = UAbilityAttributeSet::GetOutPotencyAttribute();
	HitChanceAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	HitChanceAttributeDef.AttributeToCapture = UAbilityAttributeSet::GetOutProbabilityAttribute();
	CriticalHitChanceAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	CriticalHitChanceAttributeDef.AttributeToCapture = UAbilityAttributeSet::GetOutCriticalChanceAttribute();
	CriticalHitBonusAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	CriticalHitBonusAttributeDef.AttributeToCapture = UAbilityAttributeSet::GetOutCriticalBonusAttribute();
	
	IncomingDamageAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	IncomingDamageAttributeDef.AttributeToCapture = UHitPointsAttributeSet::GetDamageAttribute();
	
	AttackAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	AttackAttributeDef.AttributeToCapture = UAttackerAttributeSet::GetAttackAttribute();
	AttackDefenseRatioCapAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	AttackDefenseRatioCapAttributeDef.AttributeToCapture = UAttackerAttributeSet::GetAttackDefenceRatioCapAttribute();
	DefencePierceAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	DefencePierceAttributeDef.AttributeToCapture = UAttackerAttributeSet::GetDefencePierceAttribute();
	DamageMultiplierAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	DamageMultiplierAttributeDef.AttributeToCapture = UAbilityAttributeSet::GetPotencyMultiplierAttribute();

	DefenceAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	DefenceAttributeDef.AttributeToCapture = UDefenderAttributeSet::GetDefenceAttribute();
	ResistanceAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	ResistanceAttributeDef.AttributeToCapture = UDefenderAttributeSet::GetResistanceAttribute();

	GuardChanceAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	GuardChanceAttributeDef.AttributeToCapture = UDefenderAttributeSet::GetGuardChanceAttribute();
	GuardHalfAngleAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	GuardHalfAngleAttributeDef.AttributeToCapture = UDefenderAttributeSet::GetGuardHalfAngleAttribute();
	GuardDamageReductionAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	GuardDamageReductionAttributeDef.AttributeToCapture = UDefenderAttributeSet::GetGuardDamageReductionAttribute();
	GuardEffectChance.TargetTagRequirements.RequireTags.AddTag(Crys::NativeGameplayTag::Ability_State_CombatStance);
	GuardEffectChance.TargetTagRequirements.RequireTags.AddTag(Crys::NativeGameplayTag::Ability_State_Guard);

	ParryChanceAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	ParryChanceAttributeDef.AttributeToCapture = UDefenderAttributeSet::GetParryChanceAttribute();
	ParryHalfAngleAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	ParryHalfAngleAttributeDef.AttributeToCapture = UDefenderAttributeSet::GetParryHalfAngleAttribute();
	ParryEffectChance.TargetTagRequirements.RequireTags.AddTag(Crys::NativeGameplayTag::Ability_State_CombatStance);
	ParryEffectChance.TargetTagRequirements.RequireTags.AddTag(Crys::NativeGameplayTag::Ability_State_Parry);

	BlockChanceAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	BlockChanceAttributeDef.AttributeToCapture = UDefenderAttributeSet::GetBlockChanceAttribute();
	BlockHalfAngleAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	BlockHalfAngleAttributeDef.AttributeToCapture = UDefenderAttributeSet::GetBlockHalfAngleAttribute();
	BlockDamageReductionAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	BlockDamageReductionAttributeDef.AttributeToCapture = UDefenderAttributeSet::GetBlockDamageReductionAttribute();
	BlockEffectChance.TargetTagRequirements.RequireTags.AddTag(Crys::NativeGameplayTag::Ability_State_Block);
	
	UpdateAggregatedRelevantAttributesToCapture();
}

void UDamageExecCalc::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	FGameplayEffectSpec* Spec = ExecutionParams.GetOwningSpecForPreExecuteMod();
	FCrimGameplayEffectContext* Context = static_cast<FCrimGameplayEffectContext*>(Spec->GetContext().Get());
	FAbilityDamageContext* AbilityDamageContext = Context->AddCustomDataFragment(FAbilityDamageContext());
	AbilityDamageContext->DamagedAttribute = IncomingDamageAttributeDef.AttributeToCapture;
	
	FAggregatorEvaluateParameters EvaluateParams;
	EvaluateParams.SourceTags = Spec->CapturedSourceTags.GetAggregatedTags();
	EvaluateParams.TargetTags = Spec->CapturedTargetTags.GetAggregatedTags();

	/** Determine if the Damage will hit the target. */
	AbilityDamageContext->bHit = IsHit(ExecutionParams, EvaluateParams);
	
	if (AbilityDamageContext->bHit)
	{
		OutExecutionOutput.MarkConditionalGameplayEffectsToTrigger();
		AbilityDamageContext->bParried = IsParried(ExecutionParams, EvaluateParams);
		if (!AbilityDamageContext->bParried)
		{
			float BaseDamage = 0.f;
			ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(BaseDamageAttributeDef, EvaluateParams, BaseDamage);
			BaseDamage = FMath::Floor(BaseDamage);
			
			AbilityDamageContext->bCriticalHit = IsCriticalHit(ExecutionParams, EvaluateParams);
			float Damage = FMath::Floor(CalculateDamage(BaseDamage, AbilityDamageContext, ExecutionParams, OutExecutionOutput, EvaluateParams));
			
			AbilityDamageContext->bBlocked = IsBlocked(ExecutionParams, EvaluateParams);
			if (AbilityDamageContext->bBlocked)
			{
				float BlockDamageReduction = 0.f;
				ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(BlockDamageReductionAttributeDef, EvaluateParams, BlockDamageReduction);
				Damage = FMath::Floor(Damage * (1 - BlockDamageReduction));
			}
			
			ExecutionParams.AttemptCalculateCapturedAttributeMagnitudeWithBase(IncomingDamageAttributeDef, EvaluateParams, Damage, Damage);
			Damage = FMath::Floor(Damage);
			OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(IncomingDamageAttributeDef.AttributeToCapture, EGameplayModOp::Override, Damage));
		}
	}
}

const TArray<FGameplayEffectAttributeCaptureDefinition>& UDamageExecCalc::GetAttributeCaptureDefinitions() const
{
	return AggregatedRelevantAttributesToCapture;
}

#if WITH_EDITOR
void UDamageExecCalc::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	
	UpdateAggregatedRelevantAttributesToCapture();
}
#endif

void UDamageExecCalc::UpdateAggregatedRelevantAttributesToCapture()
{
	AggregatedRelevantAttributesToCapture.Empty();
	AggregatedRelevantAttributesToCapture.Add(BaseDamageAttributeDef);
	AggregatedRelevantAttributesToCapture.Add(HitChanceAttributeDef);
	AggregatedRelevantAttributesToCapture.Add(CriticalHitChanceAttributeDef);
	AggregatedRelevantAttributesToCapture.Add(DefencePierceAttributeDef);
	
	AggregatedRelevantAttributesToCapture.Add(IncomingDamageAttributeDef);
	
	AggregatedRelevantAttributesToCapture.Add(AttackAttributeDef);
	AggregatedRelevantAttributesToCapture.Add(AttackDefenseRatioCapAttributeDef);
	AggregatedRelevantAttributesToCapture.Add(CriticalHitChanceAttributeDef);
	AggregatedRelevantAttributesToCapture.Add(CriticalHitBonusAttributeDef);
	AggregatedRelevantAttributesToCapture.Add(DefencePierceAttributeDef);
	AggregatedRelevantAttributesToCapture.Add(DamageMultiplierAttributeDef);
	
	AggregatedRelevantAttributesToCapture.Add(DefenceAttributeDef);
	AggregatedRelevantAttributesToCapture.Add(ResistanceAttributeDef);
	
	AggregatedRelevantAttributesToCapture.Add(GuardChanceAttributeDef);
	AggregatedRelevantAttributesToCapture.Add(GuardHalfAngleAttributeDef);
	AggregatedRelevantAttributesToCapture.Add(GuardDamageReductionAttributeDef);
	
	AggregatedRelevantAttributesToCapture.Add(ParryChanceAttributeDef);
	AggregatedRelevantAttributesToCapture.Add(ParryHalfAngleAttributeDef);
	
	AggregatedRelevantAttributesToCapture.Add(BlockChanceAttributeDef);
	AggregatedRelevantAttributesToCapture.Add(BlockHalfAngleAttributeDef);
	AggregatedRelevantAttributesToCapture.Add(BlockDamageReductionAttributeDef);
	
	AggregatedRelevantAttributesToCapture.Append(RelevantAttributesToCapture);
}

float UDamageExecCalc::CalculateDamage(const float BaseDamage, FAbilityDamageContext* AbilityDamageContext, const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput, const FAggregatorEvaluateParameters& EvaluateParams) const
{
	float Attack = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AttackAttributeDef, EvaluateParams, Attack);
	Attack = FMath::Max(1.f, Attack);
	
	float DefensePierce = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DefencePierceAttributeDef, EvaluateParams, DefensePierce);
	float Defense = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DefenceAttributeDef, EvaluateParams, Defense);
	Defense = FMath::Max(1.f, Defense * (1 - DefensePierce));
	
	float DamageRatioCap = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AttackDefenseRatioCapAttributeDef, EvaluateParams, DamageRatioCap);
	float DamageRatio = FMath::Min(Attack/Defense, DamageRatioCap);
	
	float DamageRatioCritBonus = 0.f;
	if (AbilityDamageContext->bCriticalHit)
	{
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CriticalHitBonusAttributeDef, EvaluateParams, DamageRatioCritBonus);
	}
	
	float GuardDamageReduction = 0.f;
	AbilityDamageContext->bGuarded = IsGuarded(ExecutionParams, EvaluateParams);
	if (AbilityDamageContext->bGuarded)
	{
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GuardDamageReductionAttributeDef, EvaluateParams, GuardDamageReduction);
	}
	
	DamageRatio = FMath::Max(DamageRatio + DamageRatioCritBonus - GuardDamageReduction, 0.f);
	
	float DamageMultiplier = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageMultiplierAttributeDef, EvaluateParams, DamageMultiplier);
	float Resistance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(ResistanceAttributeDef, EvaluateParams, Resistance);
	
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	const float DamageRandomizerMultiplier = FMath::FRandRange(DamageRandomizerMin.GetValueAtLevel(Spec.GetLevel()), DamageRandomizerMax.GetValueAtLevel(Spec.GetLevel()));
	
	return BaseDamage * DamageMultiplier * DamageRatio * (1 - Resistance) * DamageRandomizerMultiplier;
}

bool UDamageExecCalc::IsHit(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FAggregatorEvaluateParameters& EvaluateParams) const
{
	float HitChance = 1.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(HitChanceAttributeDef, EvaluateParams, HitChance);
	
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	return HitEffectChance.CalculateIsSuccess(HitChance, Spec.GetLevel(), *EvaluateParams.SourceTags, *EvaluateParams.TargetTags);
}

bool UDamageExecCalc::IsCriticalHit(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FAggregatorEvaluateParameters& EvaluateParams) const
{
	float CriticalHitChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CriticalHitChanceAttributeDef, EvaluateParams, CriticalHitChance);
	
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	return CriticalHitEffectChance.CalculateIsSuccess(CriticalHitChance, Spec.GetLevel(), *EvaluateParams.SourceTags, *EvaluateParams.TargetTags);
}

bool UDamageExecCalc::IsParried(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FAggregatorEvaluateParameters& EvaluateParams) const
{
	float ParryChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(ParryChanceAttributeDef, EvaluateParams, ParryChance);
	
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	const bool bParried = ParryEffectChance.CalculateIsSuccess(ParryChance, Spec.GetLevel(), *EvaluateParams.SourceTags, *EvaluateParams.TargetTags);
	
	if (bParried)
	{
		AActor* SourceActor = ExecutionParams.GetSourceAbilitySystemComponent()->GetAvatarActor();
		AActor* TargetActor = ExecutionParams.GetTargetAbilitySystemComponent()->GetAvatarActor();
		float ConeHalfAngle = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(ParryHalfAngleAttributeDef, EvaluateParams, ConeHalfAngle);
		
		if (UCrimMathStatics::IsInCone(TargetActor->GetActorLocation(), TargetActor->GetActorForwardVector(), ConeHalfAngle, SourceActor->GetActorLocation()))
		{
			return true;
		}
	}
	
	return false;
}

bool UDamageExecCalc::IsGuarded(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FAggregatorEvaluateParameters& EvaluateParams) const
{
	float GuardChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GuardChanceAttributeDef, EvaluateParams, GuardChance);
			
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	const bool bGuarded = GuardEffectChance.CalculateIsSuccess(GuardChance, Spec.GetLevel(), *EvaluateParams.TargetTags, *EvaluateParams.SourceTags);
	
	if (bGuarded)
	{
		AActor* SourceActor = ExecutionParams.GetSourceAbilitySystemComponent()->GetAvatarActor();
		AActor* TargetActor = ExecutionParams.GetTargetAbilitySystemComponent()->GetAvatarActor();
		float ConeHalfAngle = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GuardHalfAngleAttributeDef, EvaluateParams, ConeHalfAngle);
		
		if (UCrimMathStatics::IsInCone(TargetActor->GetActorLocation(), TargetActor->GetActorForwardVector(), ConeHalfAngle, SourceActor->GetActorLocation()))
		{
			return true;
		}
	}
	return false;
}

bool UDamageExecCalc::IsBlocked(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FAggregatorEvaluateParameters& EvaluateParams) const
{
	float BlockChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(BlockChanceAttributeDef, EvaluateParams, BlockChance);
	
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	const bool bBlocked = BlockEffectChance.CalculateIsSuccess(BlockChance, Spec.GetLevel(), *EvaluateParams.SourceTags, *EvaluateParams.TargetTags);
	
	if (bBlocked)
	{
		AActor* SourceActor = ExecutionParams.GetSourceAbilitySystemComponent()->GetAvatarActor();
		AActor* TargetActor = ExecutionParams.GetTargetAbilitySystemComponent()->GetAvatarActor();
		float ConeHalfAngle = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(BlockHalfAngleAttributeDef, EvaluateParams, ConeHalfAngle);
		
		if (UCrimMathStatics::IsInCone(TargetActor->GetActorLocation(), TargetActor->GetActorForwardVector(), ConeHalfAngle, SourceActor->GetActorLocation()))
		{
			return true;
		}
	}
	
	return false;
}

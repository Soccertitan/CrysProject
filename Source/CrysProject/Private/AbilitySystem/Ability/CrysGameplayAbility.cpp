// Copyright Soccertitan 2026


#include "AbilitySystem/Ability/CrysGameplayAbility.h"

#include "CrimAbilitySystemBlueprintFunctionLibrary.h"
#include "CrimAbilitySystemComponent.h"
#include "CrimMathStatics.h"
#include "CrysBlueprintFunctionLibrary.h"
#include "AbilitySystem/CrysAbilityBlueprintFunctionLibrary.h"
#include "AbilitySystem/AttributeSet/AbilityAttributeSet.h"


UCrysGameplayAbility::UCrysGameplayAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	bServerRespectsRemoteAbilityCancellation = false;
	
	TargetType = EAbilityTargetType::Any;
	TargetMaxDistance = 300.f;
	TargetHalfAngle = 45.f;
	CastTime = 0.f;
}

float UCrysGameplayAbility::GetTargetHalfAngle_Implementation() const
{
	return TargetHalfAngle.GetValueAtLevel(GetAbilityLevel());
}

float UCrysGameplayAbility::GetTargetMaxDistance_Implementation() const
{
	return TargetMaxDistance.GetValueAtLevel(GetAbilityLevel());
}

float UCrysGameplayAbility::GetCastTime_Implementation() const
{
	float BaseCastTime = CastTime.GetValueAtLevel(GetAbilityLevel());
	FGameplayTagRequirements SourceTagRequirements;
	SourceTagRequirements.RequireTags.AppendTags(GetAbilitySystemComponentFromActorInfo()->GetOwnedGameplayTags());
	SourceTagRequirements.RequireTags.AppendTags(GetAssetTags());
	float CastTimeMultiplier = GetAbilitySystemComponentFromActorInfo()->GetFilteredAttributeValue(
		UAbilityAttributeSet::GetCastSpeedMultiplierAttribute(), 
		SourceTagRequirements,
		FGameplayTagContainer());
	
	return FMath::Max(BaseCastTime * CastTimeMultiplier, 0.f);
}

void UCrysGameplayAbility::ApplyGameplayEffectToTarget(AActor* TargetActor, AActor* EffectCauser)
{
	//TODO: Implement
}

float UCrysGameplayAbility::GetCooldownTime() const
{
	float BaseCooldownTime = Super::GetCooldownTime();
	FGameplayTagRequirements SourceTagRequirements;
	SourceTagRequirements.RequireTags.AppendTags(GetAbilitySystemComponentFromActorInfo()->GetOwnedGameplayTags());
	SourceTagRequirements.RequireTags.AppendTags(GetAssetTags());
	float BaseCooldownTimeMultiplier = GetAbilitySystemComponentFromActorInfo()->GetFilteredAttributeValue(
		UAbilityAttributeSet::GetAbilityCooldownMultiplierAttribute(), 
		SourceTagRequirements,
		FGameplayTagContainer());
	
	return FMath::Max(BaseCooldownTime * BaseCooldownTimeMultiplier, 0.f);
}

bool UCrysGameplayAbility::IsPrimaryTargetValid(AActor* TargetActor) const
{
	return IsTargetValid(GetAvatarActorFromActorInfo()->GetActorLocation(), 
		GetAvatarActorFromActorInfo()->GetActorForwardVector(), TargetActor, GetTargetMaxDistance(), 
		GetTargetHalfAngle(), GetTargetType(), GetTargetTagRequirements());
}

bool UCrysGameplayAbility::IsTargetValid(const FVector Origin, const FVector ForwardVector, AActor* TargetActor, const float InMaxDistance, const float InHalfAngle, 
                                         const EAbilityTargetType InTargetType, const FGameplayTagRequirements InTargetTagRequirements) const
{
	if (!TargetActor)
	{
		return false;
	}
	
	UCrimAbilitySystemComponent* TargetASC = UCrimAbilitySystemBlueprintFunctionLibrary::GetAbilitySystemComponent(TargetActor);
	if (!TargetASC)
	{
		return false;
	}
	
	if (InTargetTagRequirements.RequirementsMet(TargetASC->GetOwnedGameplayTags()) == false)
	{
		return false;
	}
	
	if (!IsTargetOfAbilityTargetType(TargetActor, InTargetType))
	{
		return false;
	}
	
	if (FVector::Dist(Origin, TargetActor->GetActorLocation()) > InMaxDistance)
	{
		return false;
	}
	
	if (!UCrimMathStatics::IsInCone(
		Origin, 
		ForwardVector,
		InHalfAngle,
		TargetActor->GetActorLocation()))
	{
		return false;
	}
	
	return true;
}

bool UCrysGameplayAbility::IsTargetOfAbilityTargetType(AActor* TargetActor, const EAbilityTargetType InTargetType) const
{
	return UCrysAbilityBlueprintFunctionLibrary::IsAbilityTargetType(InTargetType, GetAvatarActorFromActorInfo(), TargetActor);
}

bool UCrysGameplayAbility::IsTargetWithinRange(AActor* TargetActor, float Range) const
{
	return UCrimMathStatics::DistanceBetweenActors(GetAvatarActorFromActorInfo(), TargetActor) > Range;
}

bool UCrysGameplayAbility::DoesTargetMeetTargetTagRequirements(AActor* TargetActor, const FGameplayTagRequirements& Requirements) const
{
	if (TargetActor)
	{
		if (UCrimAbilitySystemComponent* TargetASC = UCrimAbilitySystemBlueprintFunctionLibrary::GetAbilitySystemComponent(TargetActor))
		{
			return Requirements.RequirementsMet(TargetASC->GetOwnedGameplayTags());
		}
	}
	return false;
}

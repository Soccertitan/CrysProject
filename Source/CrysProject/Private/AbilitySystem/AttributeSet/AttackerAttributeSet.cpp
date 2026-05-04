// Copyright Soccertitan 2026


#include "AbilitySystem/AttributeSet/AttackerAttributeSet.h"

#include "Net/UnrealNetwork.h"


UAttackerAttributeSet::UAttackerAttributeSet()
{
	InitAttack(1.f);
	InitAccuracy(0.f);

	InitAttackDefenceRatioCap(3.f);
	InitCriticalHitChance(0.05f);
	InitCriticalHitBonus(1.f);
	InitAutoAttackDelay(2.f);
}

void UAttackerAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Attack, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, AutoAttackDelay, COND_OwnerOnly, REPNOTIFY_Always);
}

void UAttackerAttributeSet::ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::ClampAttribute(Attribute, NewValue);
	
	if (Attribute == GetAttackAttribute())
	{
		// Attack and Defence values get divided into each other. Ensure Attack and Defence are never less than 1.
		NewValue = FMath::Max(NewValue, 1.f);
		return;
	}
	
	if (Attribute == GetCriticalHitChanceAttribute() ||
		Attribute == GetCriticalHitBonusAttribute() ||
		Attribute == GetAttackDefenceRatioCapAttribute())
	{
		NewValue = FMath::Max(NewValue, 0.f);
		return;
	}
	
	if (Attribute == GetAutoAttackDelayAttribute())
	{
		NewValue = FMath::Max(NewValue, 0.1f);
		return;
	}
}

void UAttackerAttributeSet::OnRep_Attack(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Attack, OldValue);
}

void UAttackerAttributeSet::OnRep_AutoAttackDelay(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, AutoAttackDelay, OldValue);
}

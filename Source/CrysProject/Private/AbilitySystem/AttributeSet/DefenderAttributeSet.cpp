// Copyright Soccertitan 2026


#include "AbilitySystem/AttributeSet/DefenderAttributeSet.h"

#include "Net/UnrealNetwork.h"

UDefenderAttributeSet::UDefenderAttributeSet()
{
	InitDefence(1.f);
	
	InitGuardHalfAngle(45.f);
	InitGuardDamageReduction(1.f);
	InitParryHalfAngle(45.f);
	InitBlockHalfAngle(45.f);
}

void UDefenderAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Defence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, EvasionSkill, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Resistance, COND_None, REPNOTIFY_Always);
	
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, GuardSkill, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, ParrySkill, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, ShieldSkill, COND_None, REPNOTIFY_Always);
}

void UDefenderAttributeSet::ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::ClampAttribute(Attribute, NewValue);
	
	if (Attribute == GetDefenceAttribute())
	{
		// Attack and Defence values get divided into each other. Ensure Attack and Defence are never less than 1.
		NewValue = FMath::Max(NewValue, 1.f);
		return;
	}
	
	if (Attribute == GetBlockDamageReductionAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, 1.0f);
		return;
	}
	
	if (Attribute == GetResistanceAttribute())
	{
		// Don't clamp resistance.
		return;
	}
	
	NewValue = FMath::Max(NewValue, 0.f);
}

void UDefenderAttributeSet::OnRep_Defence(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Defence, OldValue);
}

void UDefenderAttributeSet::OnRep_EvasionSkill(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, EvasionSkill, OldValue);
}

void UDefenderAttributeSet::OnRep_Resistance(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Resistance, OldValue);
}

void UDefenderAttributeSet::OnRep_GuardSkill(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, GuardSkill, OldValue);
}

void UDefenderAttributeSet::OnRep_ParrySkill(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, ParrySkill, OldValue);
}

void UDefenderAttributeSet::OnRep_ShieldSkill(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, ShieldSkill, OldValue);
}

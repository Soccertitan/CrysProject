// Copyright Soccertitan 2026


#include "AbilitySystem/AttributeSet/JobAttributeSet.h"

#include "Net/UnrealNetwork.h"


UJobAttributeSet::UJobAttributeSet()
{
	InitMainJobLevel(1.f);
	InitSubJobEffectiveness(0.25f);
}

void UJobAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, MainJobLevel, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, SubJobLevel, COND_None, REPNOTIFY_Always);
}

void UJobAttributeSet::ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute == GetMainJobLevelAttribute())
	{
		NewValue = FMath::Max(NewValue, 1.f);
	}
	else
	{
		NewValue = FMath::Max(NewValue, 0.f);
	}
}

void UJobAttributeSet::OnRep_MainJobLevel(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, MainJobLevel, OldValue);
}

void UJobAttributeSet::OnRep_SubJobLevel(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, SubJobLevel, OldValue);
}

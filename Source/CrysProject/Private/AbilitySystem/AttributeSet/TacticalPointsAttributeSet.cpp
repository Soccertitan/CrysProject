// Copyright Soccertitan 2026


#include "AbilitySystem/AttributeSet/TacticalPointsAttributeSet.h"

#include "CrysNativeGameplayTags.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"


UTacticalPointsAttributeSet::UTacticalPointsAttributeSet()
{
	InitCurrentPoints(0.f);
	InitMaxPoints(3000.f);
}

void UTacticalPointsAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, CurrentPoints, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, MaxPoints, COND_None, REPNOTIFY_Always);
}

void UTacticalPointsAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	
	if (Data.EvaluatedData.Attribute == GetCurrentPointsAttribute())
	{
		SetCurrentPoints(FMath::Clamp(GetCurrentPoints(), 0.f, GetMaxPoints()));
	}
	else if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		if (Data.EvaluatedData.Magnitude > 0.f)
		{
			HandleDamage(Data, Data.EvaluatedData.Magnitude);
		}
		else
		{
			HandleHealing(Data, Data.EvaluatedData.Magnitude);
		}
		SetDamage(0.f);
	}
	else if (Data.EvaluatedData.Attribute == GetHealingAttribute())
	{
		if (Data.EvaluatedData.Magnitude > 0.f)
		{
			HandleHealing(Data, Data.EvaluatedData.Magnitude);
		}
		else
		{
			HandleDamage(Data, Data.EvaluatedData.Magnitude);
		}
		SetHealing(0.f);
	}
}

void UTacticalPointsAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
	
	if (Attribute == GetMaxPointsAttribute())
	{
		// Make sure CurrentPoints is not greater than the new MaxPoints.
		if (GetCurrentPoints() > NewValue)
		{
			GetOwningAbilitySystemComponentChecked()->SetNumericAttributeBase(GetCurrentPointsAttribute(), NewValue);
		}
	}
}

void UTacticalPointsAttributeSet::OnRep_CurrentPoints(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, CurrentPoints, OldValue);
}

void UTacticalPointsAttributeSet::OnRep_MaxPoints(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, MaxPoints, OldValue);
}

void UTacticalPointsAttributeSet::ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::ClampAttribute(Attribute, NewValue);
	
	if (Attribute == GetCurrentPointsAttribute())
	{
		// Do not allow CurrentPoints to go negative or above MaxPoints.
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxPoints());
	}
	else if (Attribute == GetMaxPointsAttribute())
	{
		// Do not allow MaxPoints to drop below 0.
		NewValue = FMath::Max(NewValue, 0.0f);
	}
}

void UTacticalPointsAttributeSet::HandleDamage(const FGameplayEffectModCallbackData& Data, float Magnitude)
{
	const float LocalDamage = FMath::Abs(Magnitude);

	SetCurrentPoints(FMath::Clamp(GetCurrentPoints() - LocalDamage, 0.f, GetMaxPoints()));
	SendGameplayEvent(Crys::NativeGameplayTag::Ability_GameplayEvent_Damage_TacticalPoints, Data, LocalDamage);
}

void UTacticalPointsAttributeSet::HandleHealing(const FGameplayEffectModCallbackData& Data, float Magnitude)
{
	const float LocalHealing = FMath::Abs(Magnitude);

	SetCurrentPoints(FMath::Clamp(GetCurrentPoints() + LocalHealing, 0.f, GetMaxPoints()));
	SendGameplayEvent(Crys::NativeGameplayTag::Ability_GameplayEvent_Healing_TacticalPoints, Data, LocalHealing);
}

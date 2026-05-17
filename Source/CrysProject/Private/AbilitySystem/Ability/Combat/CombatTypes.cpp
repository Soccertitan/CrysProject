// Copyright Soccertitan 2026


#include "AbilitySystem/Ability/Combat/CombatTypes.h"


void FCrysWeapon::SetLevel(float InValue)
{
	Level = InValue;
	GenerateTotalAttackProbabilities();
}

float FCrysWeapon::GetDamage() const
{
	return Damage.GetValueAtLevel(Level);
}

void FCrysWeapon::SetDamage(const FScalableFloat& InValue)
{
	Damage = InValue;
}

float FCrysWeapon::GetRange() const
{
	return Range.GetValueAtLevel(Level);
}

void FCrysWeapon::SetRange(const FScalableFloat& InValue)
{
	Range = InValue;
}

float FCrysWeapon::GetAutoAttackDelay() const
{
	return AutoAttackDelay.GetValueAtLevel(Level);
}

void FCrysWeapon::SetAutoAttackDelay(const FScalableFloat& InValue)
{
	AutoAttackDelay = InValue;
}

const TArray<FMultiAttackProbability>& FCrysWeapon::GetMultiAttackProbabilities() const
{
	return MultiAttackProbabilities;
}

void FCrysWeapon::SetMultiAttackProbabilities(const TArray<FMultiAttackProbability>& InValue)
{
	MultiAttackProbabilities = InValue;
	GenerateTotalAttackProbabilities();
}

int32 FCrysWeapon::CalculateBonusAttacks() const
{
	int32 Result = 0;
	float HitValue = FMath::FRandRange(0.f, TotalMultiAttackProbabilities);
	float RunningValue = 0.f;
	for (int32 Index = 0; Index < MultiAttackProbabilities.Num(); Index++)
	{
		// Count up until we find the first item that exceeds the HitValue.
		RunningValue += MultiAttackProbabilities[Index].Probability.GetValueAtLevel(Level);
		if (HitValue <= RunningValue)
		{
			Result = MultiAttackProbabilities[Index].NumOfBonusAttacks;
			break;
		}
	}
	return Result;
}

void FCrysWeapon::PostSerialize(const FArchive& Ar)
{
	GenerateTotalAttackProbabilities();
}

void FCrysWeapon::GenerateTotalAttackProbabilities()
{
	TotalMultiAttackProbabilities = 0;
	for (const FMultiAttackProbability& Probability : MultiAttackProbabilities)
	{
		TotalMultiAttackProbabilities += Probability.Probability.GetValueAtLevel(Level);
	}
}

FMultiAttackPriority::FMultiAttackPriority(const FGameplayTag& Tag, int32 BonusAttacks)
{
	MultiAttackTag = Tag;
	BonusAttackCount = BonusAttacks;
}

// Copyright Soccertitan 2026


#include "AbilitySystem/Ability/Combat/CombatTypes.h"


FMultiAttackPriority::FMultiAttackPriority(const FGameplayTag& Tag, int32 BonusAttacks)
{
	MultiAttackTag = Tag;
	BonusAttackCount = BonusAttacks;
}

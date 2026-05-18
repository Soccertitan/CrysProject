// Copyright Soccertitan 2026


#include "AbilitySystem/Ability/Combat/CombatBlueprintFunctionLibrary.h"

#include "AbilitySystem/Ability/Combat/CombatTypes.h"


float UCombatBlueprintFunctionLibrary::GetWeaponDamage(const FCrysWeapon& Weapon)
{
	return Weapon.GetDamage();
}

float UCombatBlueprintFunctionLibrary::GetWeaponRange(const FCrysWeapon& Weapon)
{
	return Weapon.GetRange();
}

float UCombatBlueprintFunctionLibrary::GetWeaponDelay(const FCrysWeapon& Weapon)
{
	return Weapon.GetAutoAttackDelay();
}

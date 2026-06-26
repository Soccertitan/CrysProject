// Copyright Soccertitan 2026


#include "AbilitySystem/Ability/Combat/CombatBlueprintFunctionLibrary.h"

#include "AbilitySystem/Ability/Combat/CombatSystemComponent.h"
#include "AbilitySystem/Ability/Combat/CombatSystemInterface.h"
#include "AbilitySystem/Ability/Combat/CombatTypes.h"


UCombatSystemComponent* UCombatBlueprintFunctionLibrary::GetCombatSystemComponent(AActor* Actor)
{
	if (!IsValid(Actor))
	{
		return nullptr;
	}

	if (Actor->Implements<UCombatSystemInterface>())
	{
		return ICombatSystemInterface::Execute_GetCombatSystemComponent(Actor);
	}

	return Actor->FindComponentByClass<UCombatSystemComponent>();
}

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

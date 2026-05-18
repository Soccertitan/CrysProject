// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CombatBlueprintFunctionLibrary.generated.h"

struct FCrysWeapon;
/**
 * 
 */
UCLASS()
class CRYSPROJECT_API UCombatBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, Category = "Combat", DisplayName = "GetDamage")
	static float GetWeaponDamage(const FCrysWeapon& Weapon);
	
	UFUNCTION(BlueprintPure, Category = "Combat", DisplayName = "GetRange")
	static float GetWeaponRange(const FCrysWeapon& Weapon);
	
	UFUNCTION(BlueprintPure, Category = "Combat", DisplayName = "GetAutoAttackDelay")
	static float GetWeaponDelay(const FCrysWeapon& Weapon);
};

// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ScalableFloat.h"
#include "Abilities/GameplayAbilityTargetTypes.h"

#include "CombatTypes.generated.h"

class UGameplayEffect;

/** Runtime data used for combat/auto attacks. */
USTRUCT(BlueprintType)
struct FCrysWeapon
{
	GENERATED_BODY()
	
	/** The level of the weapon that affects all the Weapon properties. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Level = 0;
	
	/** The skill the weapon uses to determine effectiveness. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Categories = "WeaponSkill"))
	FGameplayTag WeaponSkill;
	
	/** The base damage of the weapon. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FScalableFloat Damage = 1.f;
	
	/** The auto attack range. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FScalableFloat Range = 300.f;
	
	/** Auto attack delay in seconds. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FScalableFloat AutoAttackDelay = 2.5f;
	
	/** This value is checked via a random number between 0.f and 1.f. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FScalableFloat MultiAttackChance;
	
	/** An index of 0 is 1 additional attack, every index after counts as 1 more attack. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 0.f))
	TArray<float> MultiAttackDistribution;
	
	/** The damage effect class to use during an auto attack. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> DamageGameplayEffect;
};

USTRUCT()
struct FMultiAttackPriority
{
	GENERATED_BODY()
	
	FMultiAttackPriority(){}
	FMultiAttackPriority(const FGameplayTag& Tag, int32 BonusAttacks);
	
	UPROPERTY(EditAnywhere)
	FGameplayTag MultiAttackTag;
	
	UPROPERTY(EditAnywhere, meta = (ClampMin = 1))
	int32 BonusAttackCount = 1;
};

/** Weapon overrides the Default weapon. */
USTRUCT()
struct FCrysWeaponHandle
{
	GENERATED_BODY()
	
	uint32 GetHandle() const { return Handle; }
	FCrysWeapon GetWeapon() const { return Weapon; }
	
private:
	uint32 Handle = 0;
	
	FCrysWeapon Weapon;
	
	friend struct FCrysWeaponContainer;
};

/** Used by the WeaponManagerComponent to track currently equipped weapons. */
USTRUCT()
struct FCrysWeaponContainer
{
	GENERATED_BODY()
	
	/** Scales with the PrimaryAttributeSet::LevelAttribute. */
	UPROPERTY(EditAnywhere, NotReplicated)
	FCrysWeapon DefaultWeapon;
	
	UPROPERTY()
	bool bUseWeaponOverride = false;
	
	UPROPERTY()
	FCrysWeapon WeaponOverride;
};

USTRUCT()
struct FAutoAttackParams
{
	GENERATED_BODY()
	
	UPROPERTY()
	FCrysWeapon PrimaryWeapon;
	UPROPERTY()
	int32 PrimaryAttacks = 0;
	UPROPERTY()
	FCrysWeapon SecondaryWeapon;
	int32 SecondaryAttacks = 0;
	UPROPERTY()
	FRandomStream RandomStream;
};

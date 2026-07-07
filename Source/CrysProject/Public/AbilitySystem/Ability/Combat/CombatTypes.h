// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ScalableFloat.h"
#include "Abilities/GameplayAbilityTargetTypes.h"

#include "CombatTypes.generated.h"

class UGameplayEffect;

USTRUCT(BlueprintType)
struct FMultiAttackProbability
{
	GENERATED_BODY()
	
	/** The value will be scaled by Weapon Level. Should not be less than 0. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 0.f))
	FScalableFloat Probability = 0.f;
	
	/** The number of bonus attacks to do. When this is chosen. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 0))
	int32 NumOfBonusAttacks = 0;
};

/** Runtime data used for combat/auto attacks. */
USTRUCT(BlueprintType)
struct FCrysWeapon
{
	GENERATED_BODY()
	
	float GetLevel() const { return Level; }
	void SetLevel(float InValue);
	
	float GetDamage() const;
	void SetDamage(const FScalableFloat& InValue);
	
	float GetRange() const;
	void SetRange(const FScalableFloat& InValue);
	
	float GetAutoAttackDelay() const;
	void SetAutoAttackDelay(const FScalableFloat& InValue);
	
	const TArray<FMultiAttackProbability>& GetMultiAttackProbabilities() const;
	void SetMultiAttackProbabilities(const TArray<FMultiAttackProbability>& InValue);
	
	/** Returns the number of bonus attacks. */
	int32 CalculateBonusAttacks() const;
	
	void PostSerialize(const FArchive& Ar);
	
private:
	/** The level of the weapon that affects all the Weapon properties. */
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float Level = 0.f;
	
public:
	/** The skill the weapon uses to determine effectiveness. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Categories = "Attribute.Skill.Weapon"))
	FGameplayTag WeaponSkill;
	
private:
	/** The base damage of the weapon. */
	UPROPERTY(EditAnywhere)
	FScalableFloat Damage = 1.f;
	
public:
	/** The type of damage this weapon applies. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Categories = "DamageType"))
	FGameplayTag DamageType;
	
	/** The damage effect class to use during an auto attack. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> DamageGameplayEffect;

private:
	/** The auto attack range. */
	UPROPERTY(EditAnywhere)
	FScalableFloat Range = 300.f;
	
	/** Auto attack delay in seconds. */
	UPROPERTY(EditAnywhere)
	FScalableFloat AutoAttackDelay = 2.5f;
	
	/** The probabilities are added up and determine number of bonus attacks. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FMultiAttackProbability> MultiAttackProbabilities;
	
public:
	/** Maps to animation data on the character. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag AnimationTag;
	
private:
	/** Cached value when setting the level or MultiAttackProbabilities. */
	float TotalMultiAttackProbabilities = 0.f;
	
	void GenerateTotalAttackProbabilities();
};
template<>
struct TStructOpsTypeTraits<FCrysWeapon> : public TStructOpsTypeTraitsBase2<FCrysWeapon>
{
	enum
	{
		WithPostSerialize = true,
   };
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

USTRUCT(BlueprintType)
struct FAutoAttackParams
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly)
	FCrysWeapon PrimaryWeapon;
	UPROPERTY(BlueprintReadOnly)
	int32 PrimaryAttacks = 0;
	UPROPERTY(BlueprintReadOnly)
	FCrysWeapon SecondaryWeapon;
	UPROPERTY(BlueprintReadOnly)
	int32 SecondaryAttacks = 0;
	UPROPERTY(BlueprintReadOnly)
	FRandomStream RandomStream;
};

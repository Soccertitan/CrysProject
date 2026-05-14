// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/Combat/CombatTypes.h"
#include "Item/Item.h"
#include "Item/ItemDefinition.h"

#include "ItemFragment_Equipment.generated.h"

//-----------------------------------------------------------------------------------------------------------
// ItemShard and ItemFragment for a base Equipment.
//-----------------------------------------------------------------------------------------------------------

class UGameplayEffect;
class UEquipmentManagerComponent;

USTRUCT(BlueprintType)
struct FItemShard_Equipment : public FItemShard
{
	GENERATED_BODY()

	/** The current level of the equipment. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	int32 Level = 0;

	/** The EquipmentManagerComponent the item is currently equipped to. */
	UEquipmentManagerComponent* GetEquipmentManagerComponent() const;

	virtual bool IsMatching(const TInstancedStruct<FItemShard>& Shard) const override;

private:
	/** The EquipmentManagerComponent the item is currently equipped to. */
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TWeakObjectPtr<UEquipmentManagerComponent> EquipmentManagerComponent;

	friend UEquipmentManagerComponent;
};

/** The basic ItemFragment that any equipment type can derive from. */
USTRUCT(BlueprintType)
struct FItemFragment_Equipment : public FItemFragment
{
	GENERATED_BODY()

	FItemFragment_Equipment();

	/** The max level an equipment can be upgraded to. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment", meta = (ClampMin = 0))
	int32 MaxLevel = 0;

	/** The slots the equipment is allowed to be equipped in. Allowed to equip in the specified slot and all children of the tag. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment", meta = (Categories = "EquipSlot"))
	FGameplayTag EquipSlot;

	/** Unequips items in these slots when equipped and prevents other items from being equipped in the slots. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment", meta = (Categories = "EquipSlot"))
	FGameplayTagContainer BlockEquipSlots;

	/** A level Requirement of 0 means no requirement. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment", meta = (ClampMin = 0))
	int32 LevelRequirement = 0;

	/** The character must be one of the specified jobs to equip the item. If empty, all jobs are allowed. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment", meta = (Categories = "Job"))
	FGameplayTagContainer Jobs;

	/** Gameplay effect to grant applied at the item level. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment")
	TSubclassOf<UGameplayEffect> GameplayEffect;

	/** The Equipment ItemShard type to create. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment", AdvancedDisplay)
	TInstancedStruct<FItemShard_Equipment> EquipmentShard;

	virtual void SetDefaultValues(TInstancedStruct<FItem>& Item) const override;
	virtual void PostSerialize(const FArchive& Ar);
};
template<>
struct TStructOpsTypeTraits<FItemFragment_Equipment> : public TStructOpsTypeTraitsBase2<FItemFragment_Equipment>
{
	enum
	{
		WithPostSerialize = true,
   };
};

//-----------------------------------------------------------------------------------------------------------
// ItemFragment for Weapons
//-----------------------------------------------------------------------------------------------------------

/** An equipment ItemFragment that is a weapon. */
USTRUCT(BlueprintType)
struct FItemFragment_Weapon : public FItemFragment_Equipment
{
	GENERATED_BODY()

	FItemFragment_Weapon(){}

	/** The weapon that is used. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment|Weapon", meta = (ShowOnlyInnerProperties))
	FCrysWeapon Weapon;
	
	virtual void SetDefaultValues(TInstancedStruct<FItem>& Item) const override;
	virtual void PostSerialize(const FArchive& Ar) override;
};
template<>
struct TStructOpsTypeTraits<FItemFragment_Weapon> : public TStructOpsTypeTraitsBase2<FItemFragment_Weapon>
{
	enum
	{
		WithPostSerialize = true,
   };
};

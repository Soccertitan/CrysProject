// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/Ability/Combat/CombatTypes.h"
#include "Engine/DataAsset.h"
#include "EquipmentDefinition.generated.h"

class UJobContainer;
class UGameplayEffect;
/**
 * Static data for equipment.
 */
UCLASS()
class CRYSPROJECT_API UEquipmentDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	/** The max level an equipment can be upgraded to. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment", meta = (ClampMin = 0))
	int32 MaxLevel = 0;

	/** The slots the equipment is allowed to be equipped in. Allowed to equip in the specified slot and all children of the tag. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment", meta = (Categories = "EquipSlot"))
	FGameplayTag EquipSlot;

	/** Unequips items in these slots when equipped and prevents other items from being equipped in the slots. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment", meta = (Categories = "EquipSlot"))
	FGameplayTagContainer BlockEquipSlots;

	/** A level Requirement of 0 means no requirement. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment", meta = (ClampMin = 0))
	int32 LevelRequirement = 0;

	/** The character must be one of the specified jobs to equip the item. If empty, all jobs are allowed. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment", meta = (Categories = "Job"))
	TObjectPtr<UJobContainer> JobContainer;

	/** Gameplay effect to grant applied at the item level. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment")
	TSubclassOf<UGameplayEffect> GameplayEffect;
	
	UPROPERTY(EditDefaultsOnly, AssetRegistrySearchable, BlueprintReadOnly, Category = "Equipment")
	bool bWeapon = false;
	
	/** The weapon that is used. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment", meta = (EditCondition=bWeapon))
	FCrysWeapon Weapon;
	
#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};

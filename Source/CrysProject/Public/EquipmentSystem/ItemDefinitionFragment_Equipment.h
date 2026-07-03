// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "Item/Item.h"
#include "Item/ItemDefinition.h"

#include "ItemDefinitionFragment_Equipment.generated.h"

//-----------------------------------------------------------------------------------------------------------
// Fragments for a Equipment.
//-----------------------------------------------------------------------------------------------------------

class UEquipmentDefinition;
class UEquipmentManagerComponent;

USTRUCT(BlueprintType)
struct FItemFragment_Equipment : public FItemFragment
{
	GENERATED_BODY()

	/** The EquipmentManagerComponent the item is currently equipped to. */
	UEquipmentManagerComponent* GetEquipmentManagerComponent() const;

private:
	/** The EquipmentManagerComponent the item is currently equipped to. */
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TWeakObjectPtr<UEquipmentManagerComponent> EquipmentManagerComponent;

	friend UEquipmentManagerComponent;
};

/** The basic ItemFragment that any equipment type can derive from. */
USTRUCT(BlueprintType)
struct FItemDefinitionFragment_Equipment : public FItemDefinitionFragment
{
	GENERATED_BODY()

	FItemDefinitionFragment_Equipment(){}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AssetBundles="Equipment"))
	TSoftObjectPtr<UEquipmentDefinition> EquipmentDefinition;

	virtual TSubclassOf<UItemInstanceComponentViewModel> GetItemInstanceComponentViewModel() const override;
	virtual TInstancedStruct<FItemFragment> GetItemFragment() const override;
};

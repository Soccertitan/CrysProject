// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ItemFragment_Equipment.h"
#include "Net/Serialization/FastArraySerializer.h"

#include "EquipmentTypes.generated.h"


class UEquipmentManagerComponent;

//---------------------------------------------------------------------------------------------------------
// Fast Array for equipped items.
//---------------------------------------------------------------------------------------------------------

USTRUCT(BlueprintType)
struct FEquippedItem : public FFastArraySerializerItem
{
	GENERATED_BODY()

	/** The slot this item is equipped in. */
	UPROPERTY(BlueprintReadOnly)
	FGameplayTag EquipSlot;

	/** The equipped item identifier. */
	UPROPERTY(BlueprintReadOnly)
	FItemInstanceHandle ItemInstanceHandle;
	
	/** Equip slots the equipped item blocks. */
	UPROPERTY(BlueprintReadOnly)
	FGameplayTagContainer BlockedEquipSlots;

	UPROPERTY(NotReplicated)
	FActiveGameplayEffectHandle GameplayEffectHandle;

	//~ Begin of FFastArraySerializerItem
	void PostReplicatedAdd(const struct FEquippedItemsContainer& Container);
	void PreReplicatedRemove(const FEquippedItemsContainer& Container);
	//~ End of FFastArraySerializerItem

	bool IsValid() const;
};

USTRUCT(BlueprintType)
struct FEquippedItemsContainer : public FFastArraySerializer
{
	GENERATED_BODY()

	/** Returns a pointer to an EquippedItem. */
	FEquippedItem* FindItemByEquipSlot(const FGameplayTag& EquipSlot) const;
	FEquippedItem* FindItemByHandle(const FItemInstanceHandle& Handle) const;

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParams)
	{
		return FastArrayDeltaSerialize<FEquippedItem, FEquippedItemsContainer>(Items, DeltaParams, *this);
	}
	
private:
	UPROPERTY()
	TArray<FEquippedItem> Items;

	UPROPERTY(NotReplicated)
	TObjectPtr<UEquipmentManagerComponent> Owner;
	
	friend UEquipmentManagerComponent;
	friend struct FEquippedItem;
};
template<>
struct TStructOpsTypeTraits<FEquippedItemsContainer> : TStructOpsTypeTraitsBase2<FEquippedItemsContainer>
{
	enum { WithNetDeltaSerializer = true };
};

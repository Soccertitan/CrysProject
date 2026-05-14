// Copyright Soccertitan 2026


#include "EquipmentSystem/EquipmentTypes.h"

#include "EquipmentSystem/EquipmentManagerComponent.h"


void FEquippedItem::PostReplicatedAdd(const struct FEquippedItemsContainer& Container)
{
	Container.Owner->OnItemEquipped(*this);
}

void FEquippedItem::PreReplicatedRemove(const FEquippedItemsContainer& Container)
{
	Container.Owner->OnItemUnequipped(*this);
}

bool FEquippedItem::IsValid() const
{
	return EquipSlot.IsValid() && ItemInstanceHandle.IsValid();
}

FEquippedItem* FEquippedItemsContainer::FindItemByEquipSlot(const FGameplayTag& EquipSlot) const
{
	for (const FEquippedItem& EquippedItem : Items)
	{
		if (EquippedItem.EquipSlot == EquipSlot)
		{
			return const_cast<FEquippedItem*>(&EquippedItem);
		}
	}
	return nullptr;
}

FEquippedItem* FEquippedItemsContainer::FindItemByHandle(const FItemInstanceHandle& Handle) const
{
	for (const FEquippedItem& EquippedItem : Items)
	{
		if (EquippedItem.ItemInstanceHandle == Handle)
		{
			return const_cast<FEquippedItem*>(&EquippedItem);
		}
	}
	return nullptr;
}

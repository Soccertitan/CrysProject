// Copyright Soccertitan 2026


#include "EquipmentSystem/ItemFragment_Equipment.h"

#include "CrysNativeGameplayTags.h"


UEquipmentManagerComponent* FItemShard_Equipment::GetEquipmentManagerComponent() const
{
	return EquipmentManagerComponent.Get();
}

bool FItemShard_Equipment::IsMatching(const TInstancedStruct<FItemShard>& Shard) const
{
	if (FItemShard::IsMatching(Shard))
	{
		if (const FItemShard_Equipment* Ptr = Shard.GetPtr<FItemShard_Equipment>())
		{
			if (Ptr->Level != Level)
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	return true;
}

FItemFragment_Equipment::FItemFragment_Equipment()
{
	EquipmentShard.InitializeAsScriptStruct(FItemShard_Equipment::StaticStruct());
}

void FItemFragment_Equipment::SetDefaultValues(TInstancedStruct<FItem>& Item) const
{
	TInstancedStruct<FItemShard> NewShard;
	NewShard.InitializeAsScriptStruct(EquipmentShard.GetScriptStruct());
	AddItemShard(Item, NewShard);
}

void FItemFragment_Equipment::PostSerialize(const FArchive& Ar)
{
	// Ensure that the EquipSlot tags are not also in the BlockedEquipSlots.
	BlockEquipSlots.RemoveTag(EquipSlot);

	if (!StaticStruct()->IsChildOf(FItemFragment_Weapon::StaticStruct()))
	{
		if (EquipSlot == Crys::NativeGameplayTag::EquipSlot_Hand_Main ||
			EquipSlot == Crys::NativeGameplayTag::EquipSlot_Hand)
		{
			EquipSlot = FGameplayTag();
		}
	}
}

void FItemFragment_Weapon::SetDefaultValues(TInstancedStruct<FItem>& Item) const
{
	TInstancedStruct<FItemShard> NewShard;
	NewShard.InitializeAsScriptStruct(EquipmentShard.GetScriptStruct());
	NewShard.GetMutablePtr<FItemShard_Equipment>()->Level = Weapon.Level;
	AddItemShard(Item, NewShard);
}

void FItemFragment_Weapon::PostSerialize(const FArchive& Ar)
{
	Super::PostSerialize(Ar);

	// Ensure weapons can only have the Hand.Main or Hand tag.
	if (EquipSlot != Crys::NativeGameplayTag::EquipSlot_Hand_Main ||
		EquipSlot != Crys::NativeGameplayTag::EquipSlot_Hand)
	{
		EquipSlot = Crys::NativeGameplayTag::EquipSlot_Hand;
	}
}

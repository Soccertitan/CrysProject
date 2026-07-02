// Copyright Soccertitan 2026


#include "EquipmentSystem/ItemDefinitionFragment_Equipment.h"


UEquipmentManagerComponent* FItemFragment_Equipment::GetEquipmentManagerComponent() const
{
	return EquipmentManagerComponent.Get();
}

TInstancedStruct<FItemFragment> FItemDefinitionFragment_Equipment::GetItemFragment() const
{
	TInstancedStruct<FItemFragment> WeaponFragment;
	WeaponFragment.InitializeAsScriptStruct(FItemFragment_Equipment::StaticStruct());
	return WeaponFragment;
}

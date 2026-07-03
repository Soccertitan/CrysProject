// Copyright Soccertitan 2026


#include "EquipmentSystem/ItemDefinitionFragment_Equipment.h"

#include "EquipmentSystem/UI/EquipmentItemInstanceComponentViewModel.h"


UEquipmentManagerComponent* FItemFragment_Equipment::GetEquipmentManagerComponent() const
{
	return EquipmentManagerComponent.Get();
}

TSubclassOf<UItemInstanceComponentViewModel> FItemDefinitionFragment_Equipment::GetItemInstanceComponentViewModel() const
{
	return UEquipmentItemInstanceComponentViewModel::StaticClass();
}

TInstancedStruct<FItemFragment> FItemDefinitionFragment_Equipment::GetItemFragment() const
{
	TInstancedStruct<FItemFragment> WeaponFragment;
	WeaponFragment.InitializeAsScriptStruct(FItemFragment_Equipment::StaticStruct());
	return WeaponFragment;
}

// Copyright Soccertitan 2026


#include "EquipmentSystem/ItemDefinitionFragment_Equipment.h"


UEquipmentManagerComponent* FItemFragment_Equipment::GetEquipmentManagerComponent() const
{
	return EquipmentManagerComponent.Get();
}

bool FItemFragment_Equipment::IsMatching(const TInstancedStruct<FItemFragment>& Fragment) const
{
	if (FItemFragment::IsMatching(Fragment))
	{
		if (const FItemFragment_Equipment* Ptr = Fragment.GetPtr<FItemFragment_Equipment>())
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

TInstancedStruct<FItemFragment> FItemDefinitionFragment_Equipment::GetItemFragment() const
{
	TInstancedStruct<FItemFragment> WeaponFragment;
	WeaponFragment.InitializeAsScriptStruct(FItemFragment_Equipment::StaticStruct());
	return WeaponFragment;
}

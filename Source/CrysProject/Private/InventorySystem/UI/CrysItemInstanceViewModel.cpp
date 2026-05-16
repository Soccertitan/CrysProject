// Copyright Soccertitan 2026


#include "InventorySystem/UI/CrysItemInstanceViewModel.h"


void UCrysItemInstanceViewModel::SetUpgradeLevel(int32 Value)
{
	UE_MVVM_SET_PROPERTY_VALUE(UpgradeLevel, Value);
}

void UCrysItemInstanceViewModel::SetIsEquipped(bool bValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(bEquipped, bValue);
}
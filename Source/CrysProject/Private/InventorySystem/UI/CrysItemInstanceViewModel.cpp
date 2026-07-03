// Copyright Soccertitan 2026


#include "InventorySystem/UI/CrysItemInstanceViewModel.h"

#include "CrysNativeGameplayTags.h"
#include "EquipmentSystem/EquipmentManagerComponent.h"
#include "EquipmentSystem/ItemDefinitionFragment_Equipment.h"


void UCrysItemInstanceViewModel::SetUpgradeLevel(int32 Value)
{
	UE_MVVM_SET_PROPERTY_VALUE(UpgradeLevel, Value);
}

void UCrysItemInstanceViewModel::SetIsEquipped(bool bValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(bEquipped, bValue);
}

void UCrysItemInstanceViewModel::OnItemSet_Implementation(const TInstancedStruct<FItem>& Item)
{
	Super::OnItemSet_Implementation(Item);
	
	SetUpgradeLevel(Item.Get<FItem>().GameplayTagStackContainer.GetStackCount(Crys::NativeGameplayTag::Item_UpgradeLevel));
	
	if (const FItemFragment_Equipment* ItemFragment = Item.Get<FItem>().FindFragmentByType<FItemFragment_Equipment>())
	{
		SetIsEquipped(IsValid(ItemFragment->GetEquipmentManagerComponent()));
	}
	else
	{
		SetIsEquipped(false);
	}
}

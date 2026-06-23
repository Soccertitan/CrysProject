// Copyright Soccertitan 2026


#include "InventorySystem/UI/WeaponItemInstanceViewModel.h"

#include "EquipmentSystem/EquipmentDefinition.h"
#include "EquipmentSystem/ItemDefinitionFragment_Equipment.h"
#include "UI/ViewModel/UITagViewModel.h"


void UWeaponItemInstanceViewModel::SetWeapon(const FCrysWeapon& Value)
{
	Weapon = Value;
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetDamage);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetAutoAttackDelay);
}

void UWeaponItemInstanceViewModel::SetWeaponLevel(int32 Level)
{
	Weapon.SetLevel(Level);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetDamage);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetAutoAttackDelay);
}

void UWeaponItemInstanceViewModel::SetWeaponSkillViewModel(UUITagViewModel* Value)
{
	UE_MVVM_SET_PROPERTY_VALUE(WeaponSkillViewModel, Value);
}

void UWeaponItemInstanceViewModel::SetDamageTypeViewModel(UUITagViewModel* Value)
{
	UE_MVVM_SET_PROPERTY_VALUE(DamageTypeViewModel, Value);
}

void UWeaponItemInstanceViewModel::OnItemSet_Implementation(const TInstancedStruct<FItem>& Item)
{
	Super::OnItemSet_Implementation(Item);
	
	SetWeaponLevel(GetUpgradeLevel());
}

void UWeaponItemInstanceViewModel::OnItemDefinitionSet_Implementation(const UItemDefinition* ItemDefinition)
{
	Super::OnItemDefinitionSet_Implementation(ItemDefinition);
	
	const FItemDefinitionFragment_Equipment* ItemFragment = ItemDefinition->FindFragmentByType<FItemDefinitionFragment_Equipment>();
	if (ItemFragment && ItemFragment->EquipmentDefinition.Get())
	{
		const UEquipmentDefinition* EquipmentDef = ItemFragment->EquipmentDefinition.Get();
		SetWeapon(EquipmentDef->Weapon);
		SetWeaponLevel(GetUpgradeLevel());
		
		UUITagViewModel* WeaponSkill = NewObject<UUITagViewModel>(this);
		WeaponSkill->SetGameplayTag(EquipmentDef->Weapon.WeaponSkill);
		SetWeaponSkillViewModel(WeaponSkill);
		
		UUITagViewModel* DamageType = NewObject<UUITagViewModel>(this);
		DamageType->SetGameplayTag(EquipmentDef->Weapon.DamageType);
		SetDamageTypeViewModel(DamageType);
	}
}

// Copyright Soccertitan 2026


#include "InventorySystem/UI/WeaponItemInstanceViewModel.h"

#include "EquipmentSystem/ItemFragment_Equipment.h"
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
	
	if (const FItemFragment_Weapon* Fragment = ItemDefinition->FindFragmentByType<FItemFragment_Weapon>())
	{
		SetWeapon(Fragment->Weapon);
		SetWeaponLevel(GetUpgradeLevel());
		
		UUITagViewModel* WeaponSkill = NewObject<UUITagViewModel>(this);
		WeaponSkill->SetGameplayTag(Fragment->Weapon.WeaponSkill);
		SetWeaponSkillViewModel(WeaponSkill);
		
		UUITagViewModel* DamageType = NewObject<UUITagViewModel>(this);
		DamageType->SetGameplayTag(Fragment->Weapon.DamageType);
		SetDamageTypeViewModel(DamageType);
	}
}

// Copyright Soccertitan 2026


#include "InventorySystem/UI/WeaponItemInstanceViewModel.h"

#include "EquipmentSystem/ItemFragment_Equipment.h"
#include "UI/ViewModel/UITagViewModel.h"

void UWeaponItemInstanceViewModel::SetDamage(FScalableFloat Value)
{
	Damage = Value;
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetDamage);
}

void UWeaponItemInstanceViewModel::SetAutoAttackDelay(FScalableFloat Value)
{
	AutoAttackDelay = Value;
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
	
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetDamage);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetAutoAttackDelay);
}

void UWeaponItemInstanceViewModel::OnItemDefinitionSet_Implementation(const UItemDefinition* ItemDefinition)
{
	Super::OnItemDefinitionSet_Implementation(ItemDefinition);
	
	if (const FItemFragment_Weapon* Fragment = ItemDefinition->FindFragmentByType<FItemFragment_Weapon>())
	{
		SetDamage(Fragment->Weapon.Damage);
		SetAutoAttackDelay(Fragment->Weapon.AutoAttackDelay);
		
		UUITagViewModel* WeaponSkill = NewObject<UUITagViewModel>(this);
		WeaponSkill->SetGameplayTag(Fragment->Weapon.WeaponSkill);
		SetWeaponSkillViewModel(WeaponSkill);
		
		UUITagViewModel* DamageType = NewObject<UUITagViewModel>(this);
		DamageType->SetGameplayTag(Fragment->Weapon.DamageType);
		SetDamageTypeViewModel(DamageType);
	}
}

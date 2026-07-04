// Copyright Soccertitan 2026


#include "EquipmentSystem/UI/EquipmentItemInstanceComponentViewModel.h"

#include "GameplayEffect.h"
#include "AbilitySystem/GameplayEffect/Component/GameplayEffectUIData_AttributeText.h"
#include "EquipmentSystem/EquipmentDefinition.h"
#include "EquipmentSystem/EquipmentManagerComponent.h"
#include "EquipmentSystem/ItemDefinitionFragment_Equipment.h"
#include "InventorySystem/UI/CrysItemInstanceViewModel.h"
#include "JobSystem/JobContainer.h"
#include "UI/ViewModel/UITagViewModel.h"


void UEquipmentItemInstanceComponentViewModel::SetLevelRequirement(int32 Value)
{
	UE_MVVM_SET_PROPERTY_VALUE(LevelRequirement, Value);
}

void UEquipmentItemInstanceComponentViewModel::SetAttributeText(FText Value)
{
	UE_MVVM_SET_PROPERTY_VALUE(AttributeText, Value);
}

void UEquipmentItemInstanceComponentViewModel::SetAllowedJobViewModels(TArray<UUITagViewModel*> Value)
{
	AllowedJobViewModels = Value;
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetAllowedJobViewModels);
}

void UEquipmentItemInstanceComponentViewModel::SetEquipSlotViewModels(UUITagViewModel* Value)
{
	EquipSlotViewModel = Value;
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetEquipSlotViewModels);
}

void UEquipmentItemInstanceComponentViewModel::SetIsWeapon(bool bValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(bWeapon, bValue);
}

void UEquipmentItemInstanceComponentViewModel::SetWeapon(const FCrysWeapon& Value)
{
	Weapon = Value;
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetDamage);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetAutoAttackDelay);
}

void UEquipmentItemInstanceComponentViewModel::SetWeaponLevel(int32 Level)
{
	Weapon.SetLevel(Level);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetDamage);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetAutoAttackDelay);
}

void UEquipmentItemInstanceComponentViewModel::SetWeaponSkillViewModel(UUITagViewModel* Value)
{
	UE_MVVM_SET_PROPERTY_VALUE(WeaponSkillViewModel, Value);
}

void UEquipmentItemInstanceComponentViewModel::SetDamageTypeViewModel(UUITagViewModel* Value)
{
	UE_MVVM_SET_PROPERTY_VALUE(DamageTypeViewModel, Value);
}

void UEquipmentItemInstanceComponentViewModel::OnItemSet_Implementation(const TInstancedStruct<FItem>& Item)
{
	Super::OnItemSet_Implementation(Item);
	
}

void UEquipmentItemInstanceComponentViewModel::OnItemDefinitionSet_Implementation(const UItemDefinition* ItemDefinition)
{
	Super::OnItemDefinitionSet_Implementation(ItemDefinition);
	
	const FItemDefinitionFragment_Equipment* ItemDefFragment = ItemDefinition->FindFragmentByType<FItemDefinitionFragment_Equipment>();
	if (ItemDefFragment && ItemDefFragment->EquipmentDefinition.Get())
	{
		UCrysItemInstanceViewModel* ItemInstanceViewModel = Cast<UCrysItemInstanceViewModel>(GetOuter());
		const UEquipmentDefinition* EquipmentDef = ItemDefFragment->EquipmentDefinition.Get();
		SetLevelRequirement(EquipmentDef->LevelRequirement);

		UUITagViewModel* NewVM = NewObject<UUITagViewModel>(this);
		NewVM->SetGameplayTag(EquipmentDef->EquipSlot);
		SetEquipSlotViewModels(NewVM);
		
		if (EquipmentDef->JobContainer && EquipmentDef->JobContainer->Jobs.IsValid())
		{
			TArray<UUITagViewModel*> AllowedJobs;
			for (const FGameplayTag& Job : EquipmentDef->JobContainer->Jobs.GetGameplayTagArray())
			{
				UUITagViewModel* NewJobVM = NewObject<UUITagViewModel>(this);
				NewJobVM->SetGameplayTag(Job);
				AllowedJobs.Add(NewJobVM);
			}
			SetAllowedJobViewModels(AllowedJobs);
		}
		
		SetIsWeapon(EquipmentDef->bIsWeapon);
		if (EquipmentDef->bIsWeapon)
		{
			SetWeapon(EquipmentDef->Weapon);
			if (ItemInstanceViewModel)
			{
				SetWeaponLevel(ItemInstanceViewModel->GetUpgradeLevel());
			}
			else
			{
				SetWeaponLevel(0);
			}
		
			UUITagViewModel* WeaponSkill = NewObject<UUITagViewModel>(this);
			WeaponSkill->SetGameplayTag(EquipmentDef->Weapon.WeaponSkill);
			SetWeaponSkillViewModel(WeaponSkill);
		
			UUITagViewModel* DamageType = NewObject<UUITagViewModel>(this);
			DamageType->SetGameplayTag(EquipmentDef->Weapon.DamageType);
			SetDamageTypeViewModel(DamageType);
		}
		
		if (TSubclassOf<UGameplayEffect> GameplayEffectClass = EquipmentDef->GameplayEffect)
		{
			UGameplayEffect* GE = GameplayEffectClass->GetDefaultObject<UGameplayEffect>();
			if (const UGameplayEffectUIData_AttributeText* GameplayEffectUIData_AttributeText = GE->FindComponent<UGameplayEffectUIData_AttributeText>())
			{
				const int32 UpgradeLevel = ItemInstanceViewModel ? ItemInstanceViewModel->GetUpgradeLevel() : 0;
				SetAttributeText(GameplayEffectUIData_AttributeText->GetAttributeDescription(UpgradeLevel));
			}
		}
	}
}

// Copyright Soccertitan 2026


#include "InventorySystem/UI/EquipmentItemInstanceViewModel.h"

#include "EquipmentSystem/EquipmentDefinition.h"
#include "EquipmentSystem/EquipmentManagerComponent.h"
#include "EquipmentSystem/ItemDefinitionFragment_Equipment.h"
#include "JobSystem/JobContainer.h"
#include "UI/ViewModel/UITagViewModel.h"

UEquipmentItemInstanceViewModel::UEquipmentItemInstanceViewModel()
{
	Bundles.Add("Equipment");
}

void UEquipmentItemInstanceViewModel::SetLevelRequirement(int32 Value)
{
	UE_MVVM_SET_PROPERTY_VALUE(LevelRequirement, Value);
}

void UEquipmentItemInstanceViewModel::SetAllowedJobViewModels(TArray<UUITagViewModel*> Value)
{
	AllowedJobViewModels = Value;
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetAllowedJobViewModels);
}

void UEquipmentItemInstanceViewModel::SetEquipSlotViewModels(UUITagViewModel* Value)
{
	EquipSlotViewModel = Value;
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetEquipSlotViewModels);
}

void UEquipmentItemInstanceViewModel::OnItemSet_Implementation(const TInstancedStruct<FItem>& Item)
{
	Super::OnItemSet_Implementation(Item);
	
	if (const FItemFragment_Equipment* ItemFragment = Item.Get<FItem>().FindFragmentByType<FItemFragment_Equipment>())
	{
		SetUpgradeLevel(ItemFragment->Level);
		SetIsEquipped(IsValid(ItemFragment->GetEquipmentManagerComponent()));
	}
	else
	{
		SetUpgradeLevel(0);
		SetIsEquipped(false);
	}
}

void UEquipmentItemInstanceViewModel::OnItemDefinitionSet_Implementation(const UItemDefinition* ItemDefinition)
{
	Super::OnItemDefinitionSet_Implementation(ItemDefinition);
	
	const FItemDefinitionFragment_Equipment* ItemDefFragment = ItemDefinition->FindFragmentByType<FItemDefinitionFragment_Equipment>();
	if (ItemDefFragment && ItemDefFragment->EquipmentDefinition.Get())
	{
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
	}
}

// Copyright Soccertitan 2026


#include "InventorySystem/UI/EquipmentItemInstanceViewModel.h"

#include "EquipmentSystem/EquipmentManagerComponent.h"
#include "EquipmentSystem/ItemFragment_Equipment.h"
#include "UI/ViewModel/UITagViewModel.h"

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
	
	if (const FItemShard_Equipment* ItemShard = Item.Get<FItem>().FindShardByType<FItemShard_Equipment>())
	{
		SetUpgradeLevel(ItemShard->Level);
		SetIsEquipped(IsValid(ItemShard->GetEquipmentManagerComponent()));
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
	
	if (const FItemFragment_Equipment* ItemFragment = ItemDefinition->FindFragmentByType<FItemFragment_Equipment>())
	{
		SetLevelRequirement(ItemFragment->LevelRequirement);

		UUITagViewModel* NewVM = NewObject<UUITagViewModel>(this);
		NewVM->SetGameplayTag(ItemFragment->EquipSlot);
		SetEquipSlotViewModels(NewVM);
		
		TArray<UUITagViewModel*> AllowedJobs;
		for (const FGameplayTag& Job : ItemFragment->Jobs.GetGameplayTagArray())
		{
			UUITagViewModel* NewJobVM = NewObject<UUITagViewModel>(this);
			NewJobVM->SetGameplayTag(Job);
			AllowedJobs.Add(NewJobVM);
		}
		SetAllowedJobViewModels(AllowedJobs);
	}
}

// Copyright Soccertitan 2026


#include "EquipmentSystem/EquipmentSystemBlueprintFunctionLibrary.h"

#include "InventoryBlueprintFunctionLibrary.h"
#include "EquipmentSystem/EquipmentManagerComponent.h"
#include "EquipmentSystem/EquipmentSystemInterface.h"
#include "EquipmentSystem/ItemDefinitionFragment_Equipment.h"
#include "EquipmentSystem/EquipmentDefinition.h"
#include "System/CrysAssetManager.h"

UEquipmentManagerComponent* UEquipmentSystemBlueprintFunctionLibrary::GetEquipmentManagerComponent(AActor* Actor)
{
	if (!IsValid(Actor))
	{
		return nullptr;
	}

	if (Actor->Implements<UEquipmentSystemInterface>())
	{
		return IEquipmentSystemInterface::Execute_GetEquipmentManagerComponent(Actor);
	}

	return Actor->FindComponentByClass<UEquipmentManagerComponent>();
}

UEquipmentDefinition* UEquipmentSystemBlueprintFunctionLibrary::FindEquipmentDefinition(const TInstancedStruct<FItem>& Item)
{
	const UItemDefinition* ItemDefinition = UInventoryBlueprintFunctionLibrary::GetItemDefinition(Item);
	if (!ItemDefinition)
	{
		return nullptr;
	}
	
	const FItemFragment_Equipment* ItemFrag_Equipment = Item.Get().FindFragmentByType<FItemFragment_Equipment>();
	if (!ItemFrag_Equipment)
	{
		return nullptr;
	}

	const FItemDefinitionFragment_Equipment* ItemDefFrag_Equipment = ItemDefinition->FindFragmentByType<FItemDefinitionFragment_Equipment>();
	if (!ItemDefFrag_Equipment)
	{
		return nullptr;
	}
	
	return UCrysAssetManager::GetAsset(ItemDefFrag_Equipment->EquipmentDefinition, false);
}

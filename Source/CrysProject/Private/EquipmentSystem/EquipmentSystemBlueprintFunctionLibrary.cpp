// Copyright Soccertitan 2026


#include "EquipmentSystem/EquipmentSystemBlueprintFunctionLibrary.h"

#include "EquipmentSystem/EquipmentManagerComponent.h"
#include "EquipmentSystem/EquipmentSystemInterface.h"

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

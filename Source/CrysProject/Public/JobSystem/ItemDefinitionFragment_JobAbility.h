// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "EquipmentSystem/ItemDefinitionFragment_Equipment.h"


class UAbilityInput;
class UGameplayEffect;
class UJobDefinition;

USTRUCT()
struct FItemDefinitionFragment_JobAbility : public FItemDefinitionFragment
{
	GENERATED_BODY()
	
	// Maps a Job to the level the ability is usable.
	UPROPERTY(EditAnywhere)
	TMap<TObjectPtr<UJobDefinition>, int32> JobMap;
	
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UAbilityInput>
};

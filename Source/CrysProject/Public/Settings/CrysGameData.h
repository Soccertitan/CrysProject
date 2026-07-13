// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "CrysGameData.generated.h"

class UCrysGameplayTagRelationship;
class UGameplayEffect;
class UUITagRelationship;
class UAttributeTagRelationship;

/**
 * 
 */
UCLASS(Config = Game, DefaultConfig, DisplayName = "Crys Project Game Data")
class CRYSPROJECT_API UCrysGameData : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "GameplayTag")
	TSoftObjectPtr<UCrysGameplayTagRelationship> GameplayTagRelationship;
	
	// The default AutoAttack GE to use when one is not specified on the weapon.
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	TSoftClassPtr<UGameplayEffect> AutoAttackGameplayEffectClass;
};

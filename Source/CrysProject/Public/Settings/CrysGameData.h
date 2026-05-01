// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "CrysGameData.generated.h"

class UAttributeTagRelationship;

/**
 * 
 */
UCLASS(Config = Game, DefaultConfig, DisplayName = "Crys Project Game Data")
class CRYSPROJECT_API UCrysGameData : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	// Maps Gameplay tags to attributes. Used in the UI and for Gameplay Effects to set attributes.
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	TSoftObjectPtr<UAttributeTagRelationship> AttributeTagRelationship;
};

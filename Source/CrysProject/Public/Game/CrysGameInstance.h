// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CrysGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class CRYSPROJECT_API UCrysGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	/**
	 * The CrysGameMode uses this value to check which PlayerStart to go to.
	 * This can be from transitioning between maps or checkpoints upon death.
	 */
	UPROPERTY(BlueprintReadOnly)
	FName PlayerStartTag;
};

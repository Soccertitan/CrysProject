// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "JobContainer.generated.h"

/**
 * A simple asset that contains a collection of jobs. Used to have a reusable way to define which classes are allowed to do certain things.
 */
UCLASS()
class CRYSPROJECT_API UJobContainer : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment", meta = (Categories = "Job"))
	FGameplayTagContainer Jobs;
};

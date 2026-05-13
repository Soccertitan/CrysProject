// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Item/ItemDefinition.h"
#include "CrysActionTypes.generated.h"

class UCrysAction;
/** Action mappings that can be loaded into the CrysActionManagerComponent. */
USTRUCT(BlueprintType)
struct FCrysActionMap
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Input"))
	TMap<FGameplayTag, TSubclassOf<UCrysAction>> ActionMap;
};

/** Runtime data for action mappings in the CrysActionManagerComponent */
USTRUCT(BlueprintType)
struct FCrysActionMapInstance
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly)
	TMap<FGameplayTag, TObjectPtr<UCrysAction>> ActionMap;
};

/** Information on an action that can be used or mapped. */
USTRUCT(BlueprintType)
struct FItemFragment_Action : public FItemFragment
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftClassPtr<UCrysAction> Action;
};

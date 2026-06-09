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
	TMap<FGameplayTag, TSubclassOf<UCrysAction>> InputTagActionMap;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<UCrysAction>> Actions;
};

/** Runtime data for action mappings in the CrysActionManagerComponent */
USTRUCT(BlueprintType)
struct FCrysActionMapInstance
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly)
	TMap<FGameplayTag, TObjectPtr<UCrysAction>> InputTagActionMap;
	
	/** A generic array of actions that are mapped to an index. Typically, for keyboards that want to click on an action. */
	UPROPERTY()
	TArray<TObjectPtr<UCrysAction>> Actions;
};

/** Information on an action that can be used or mapped. */
USTRUCT(BlueprintType)
struct FItemFragment_Action : public FItemFragment
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftClassPtr<UCrysAction> Action;
};

UENUM(BlueprintType)
enum class ECrysActionInputMode : uint8
{
	Gamepad UMETA(DisplayName = "Gamepad"),
	Keyboard UMETA(DisplayName = "Keyboard")
};

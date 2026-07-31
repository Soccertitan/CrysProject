// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Filter/CrimFilter.h"
#include "Filter_EquipableItems.generated.h"

/**
 * Expects an EquipmentManagerComponent to be passed in as the context.
 */
UCLASS()
class CRYSPROJECT_API UFilter_EquipableItems : public UCrimFilter
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "EquipSlot"))
	FGameplayTag EquipSlot;

	virtual bool ShouldFilterObject_Implementation(const UObject* Object, const UObject* Context) const override;
};

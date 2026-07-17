// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UI/ViewModel/Filter/ItemInstanceViewModelFilter.h"
#include "ItemInstanceViewModelFilter_EquipableItems.generated.h"

/**
 * Expects an EquipmentManagerComponent to be passed in as the context.
 */
UCLASS()
class CRYSPROJECT_API UItemInstanceViewModelFilter_EquipableItems : public UItemInstanceViewModelFilter
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "EquipSlot"))
	FGameplayTag EquipSlot;

protected:
	virtual bool ShouldBeginFilter(const UObject* Context, TArray<UItemInstanceViewModel*>& ViewModels) const override;
	virtual bool ShouldFilterItemInstance(const UObject* Context, UItemInstanceViewModel* ViewModel) const override;
};

// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "CrysItemInstanceViewModel.h"
#include "EquipmentItemInstanceViewModel.generated.h"

class UUITagViewModel;
/**
 * 
 */
UCLASS()
class CRYSPROJECT_API UEquipmentItemInstanceViewModel : public UCrysItemInstanceViewModel
{
	GENERATED_BODY()
	
public:
	int32 GetLevelRequirement() const {return LevelRequirement;}
	UFUNCTION(BlueprintPure, FieldNotify)
	TArray<UUITagViewModel*> GetAllowedJobViewModels() const {return AllowedJobViewModels;}
	UFUNCTION(BlueprintPure, FieldNotify)
	UUITagViewModel* GetEquipSlotViewModels() const {return EquipSlotViewModel;}
	
protected:
	void SetLevelRequirement(int32 Value);
	void SetAllowedJobViewModels(TArray<UUITagViewModel*> Value);
	void SetEquipSlotViewModels(UUITagViewModel* Value);
	
	virtual void OnItemSet_Implementation(const TInstancedStruct<FItem>& Item) override;
	virtual void OnItemDefinitionSet_Implementation(const UItemDefinition* ItemDefinition) override;
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, FieldNotify, Getter, meta = (AllowPrivateAccess = "true"))
	int32 LevelRequirement = 0;

	UPROPERTY()
	TObjectPtr<UUITagViewModel> EquipSlotViewModel;
	
	UPROPERTY()
	TArray<TObjectPtr<UUITagViewModel>> AllowedJobViewModels;
};

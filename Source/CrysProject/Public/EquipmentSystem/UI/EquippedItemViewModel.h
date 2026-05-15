// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "MVVMViewModelBase.h"
#include "EquippedItemViewModel.generated.h"

class UUITagViewModel;
class UItemInstanceViewModel;

/**
 * 
 */
UCLASS()
class CRYSPROJECT_API UEquippedItemViewModel : public UMVVMViewModelBase
{
	GENERATED_BODY()
	
public:
	FGameplayTag GetEquipSlot() const { return EquipSlot; }
	UItemInstanceViewModel* GetItemInstanceViewModel() const { return ItemInstanceViewModel; }
	UUITagViewModel* GetEquipSlotUITagViewModel() const { return EquipSlotUITagViewModel; }
	
protected:
	void SetEquipSlot(const FGameplayTag& InValue);
	void SetItemInstanceViewModel(UItemInstanceViewModel* InValue);
	void SetEquipSlotGameplayTagViewModel(UUITagViewModel* InValue);
	
private:
	/** The EquipSlot the item is equipped to. */
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Category = "EquippedItemViewModel", meta = (AllowPrivateAccess = true))
	FGameplayTag EquipSlot;
	
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Category = "EquippedItemViewModel", meta = (AllowPrivateAccess = true))
	TObjectPtr<UItemInstanceViewModel> ItemInstanceViewModel;
	
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Category = "EquippedItemViewModel", meta = (AllowPrivateAccess = true))
	TObjectPtr<UUITagViewModel> EquipSlotUITagViewModel;
	
	friend class UEquipmentManagerViewModel;
};

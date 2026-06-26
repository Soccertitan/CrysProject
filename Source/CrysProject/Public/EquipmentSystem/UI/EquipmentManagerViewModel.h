// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UI/ViewModel/CrysViewModel.h"
#include "EquipmentManagerViewModel.generated.h"


struct FCrysWeapon;
class UCombatSystemComponent;
class UItemInstanceViewModel;
struct FItemInstance;
struct FEquippedItem;
struct FGameplayTag;
class UItemContainerViewModel;
class UEquippedItemViewModel;
class UInventoryManagerComponent;
class UEquipmentManagerComponent;
class UItemInstanceViewModelFilter_EquipableItems;

/**
 * 
 */
UCLASS()
class CRYSPROJECT_API UEquipmentManagerViewModel : public UCrysViewModel
{
	GENERATED_BODY()
	
public:
	UEquipmentManagerViewModel();
	virtual void InitializeViewModel(APlayerController* PlayerController) override;
	
	/** Finds or creates an ActionBarItem VM from InputTag. */
	UFUNCTION(BlueprintCallable, Category = "Viewmodel|EquipSystem")
	UEquippedItemViewModel* FindOrCreateEquippedItemViewModel(UPARAM(meta = (Categories = "EquipSlot")) const FGameplayTag EquipSlot);
	
	UFUNCTION(BlueprintCallable, BlueprintPure=false, Category = "Viewmodel|EquipSystem")
	TArray<UItemContainerViewModel*> GetAllowedItemContainers() const;
	
	UFUNCTION(BlueprintCallable, BlueprintPure=false, Category = "Viewmodel|EquipSystem")
	TArray<UItemInstanceViewModel*> GetEquippableItems(UPARAM(meta = (Categories = "EquipSlot")) const FGameplayTag EquipSlot, UItemContainerViewModel* ItemContainerViewModel) const;
	
	UFUNCTION(BlueprintCallable, Category = "Viewmodel|EquipSystem")
	void EquipItem(UPARAM(meta = (Categories = "EquipSlot")) FGameplayTag EquipSlot, UItemInstanceViewModel* ItemInstanceViewModel);
	
	UFUNCTION(BlueprintCallable, Category = "Viewmodel|EquipSystem")
	void UnequipItem(UPARAM(meta = (Categories = "EquipSlot")) FGameplayTag EquipSlot);
	
	FGameplayTag GetPrimaryWeaponSkill() const { return PrimaryWeaponSkill; }
	
protected:
	
private:
	UPROPERTY(EditDefaultsOnly, Instanced, NoClear)
	TObjectPtr<UItemInstanceViewModelFilter_EquipableItems> EquippableItemsFilter;
	
	UPROPERTY(BlueprintReadOnly, FieldNotify, meta = (AllowPrivateAccess = "true"))
	FGameplayTag PrimaryWeaponSkill;

	UPROPERTY()
	TObjectPtr<UEquipmentManagerComponent> EquipmentManagerComponent;
	UPROPERTY()
	TObjectPtr<UInventoryManagerComponent> InventoryManagerComponent;
	UPROPERTY()
	TObjectPtr<UCombatSystemComponent> CombatSystemComponent;
	
	UPROPERTY()
	TArray<TObjectPtr<UEquippedItemViewModel>> EquippedItemViewModels;
	UPROPERTY()
	TArray<TObjectPtr<UItemContainerViewModel>> AllowedItemContainerViewModels;
	
	UEquippedItemViewModel* InternalCreateEquippedItemViewModel(const FGameplayTag& EquipSlot);
	
	UFUNCTION()
	void OnItemEquipped(const FEquippedItem& EquippedItem);
	UFUNCTION()
	void OnItemUnequipped(const FEquippedItem& EquippedItem);
	UFUNCTION()
	void OnItemChanged(const FItemInstance& ItemInstance);
	
	UFUNCTION()
	void OnPrimaryWeaponChanged(const FCrysWeapon& Weapon);
};

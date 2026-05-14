// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "CrimAbilitySystemInterface.h"
#include "EquipmentTypes.h"
#include "Components/ActorComponent.h"
#include "EquipmentManagerComponent.generated.h"


class UCombatSystemComponent;
class UJobDefinition;
class UJobSystemComponent;
class UInventoryManagerComponent;
DECLARE_MULTICAST_DELEGATE(FEquipmentManagerComponentGenericSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEquipmentChangedSignature, const FEquippedItem&, Item);

/** 
 * Handles equipping and unequipping items. Requires an AbilitySystemComponent, an InventoryManagerComponent, and a
 * JobManagerComponent to function. It will grab the required components from the owner by default.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CRYSPROJECT_API UEquipmentManagerComponent : public UActorComponent, public ICrimAbilitySystemInterface
{
	GENERATED_BODY()

public:
	UEquipmentManagerComponent();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;
	virtual void PreNetReceive() override;

	virtual void SetCrimAbilitySystem_Implementation(UCrimAbilitySystemComponent* AbilitySystemComponent) override;

	// Called on Client + Server when an item is equipped.
	UPROPERTY(BlueprintAssignable, DisplayName = "OnItemEquipped")
	FEquipmentChangedSignature OnItemEquippedDelegate;
	// Called on Client + Server when an item is unequipped.
	UPROPERTY(BlueprintAssignable, DisplayName = "OnItemUnequipped")
	FEquipmentChangedSignature OnItemUnequippedDelegate;
	// Called after all dependencies have been met and the component is ready to equip items.
	FEquipmentManagerComponentGenericSignature OnEquipmentManagerInitializedDelegate;

	/**
	 * Tries to equip an Item at the specified EquipSlot. Will call UnequipItem if an item already is equipped in 
	 * the same slot.
	 * @param EquipSlot The slot to equip the item.
	 * @param Handle The ItemInstanceHandle for the item in the owner's InventoryManager.
	 */
	UFUNCTION(BlueprintCallable, Category = "EquipmentManager")
	void EquipItem(UPARAM(meta=(Categories="EquipSlot")) FGameplayTag EquipSlot, const FItemInstanceHandle& Handle);

	/**
	 * Unequips an item from the specified equip slot.
	 * @param EquipSlot The slot to unequip the item from.
	 */
	UFUNCTION(BlueprintCallable, Category = "EquipmentManager")
	void UnequipItem(UPARAM(meta=(Categories="EquipSlot")) FGameplayTag EquipSlot);

	/** Retrieves a copy of the ItemEquipped in slot. */
	UFUNCTION(BlueprintPure, Category = "EquipmentManager")
	FEquippedItem GetEquippedItem(const FGameplayTag& EquipSlot) const;

	/** 
	 * Checks to see if the item can be equipped based on the item's equip requirements. Does not check if a slot 
	 * is blocked by another piece of equipment.
	 */
	UFUNCTION(BlueprintPure, Category = "EquipmentManager")
	virtual bool CanEquipItem(UPARAM(ref) const TInstancedStruct<FItem>& Item) const;
	
	/** Checks if the item is allowed to be equipped at the slot. Checks if the slot is blocked and dual wield requirements.*/
	UFUNCTION(BlueprintPure, Category = "EquipmentManager")
	virtual bool CanEquipItemInSlot(UPARAM(meta=(Categories="EquipSlot")) FGameplayTag EquipSlot, UPARAM(ref) const TInstancedStruct<FItem>& Item);
	
	
	/** Returns true if the weapon can be equipped in the SubHand slot. */
	UFUNCTION(BlueprintPure, Category = "EquipmentManager")
	bool CanDualWieldWeapon(UPARAM(ref) const TInstancedStruct<FItem>& Item) const;
	
	/** Returns true if the equip slot is blocked by another item. */
	UFUNCTION(BlueprintPure, Category = "EquipmentManager")
	bool IsEquipSlotBlocked(UPARAM(meta=(Categories="EquipSlot")) const FGameplayTag EquipSlot) const;

	/** Returns true if items can be Equipped/Unequipped. Bind to OnEquipmentManagerInitializedDelegate to be notified when ready. */
	bool IsEquipmentManagerInitialized() const;
	
	// If empty, all ItemContainers are allowed.
	const FGameplayTagContainer& GetAllowedItemContainers() const {return AllowedItemContainers;}
	
	/* Returns true if this Component's Owner Actor has authority. */
	UFUNCTION(BlueprintPure, Category = "EquipmentManager")
	bool HasAuthority() const;

protected:
	/** If empty, will check all ItemContainers in the Inventory Manager.*/
	UPROPERTY(EditDefaultsOnly, meta = (Categories = "ItemContainer"))
	FGameplayTagContainer AllowedItemContainers;
	
	virtual void OnRegister() override;
	
	/** Called when an item is equipped. */
	virtual void OnItemEquipped(const FEquippedItem& EquippedItem);
	/** Called when an item is unequipped. */
	virtual void OnItemUnequipped(const FEquippedItem& EquippedItem);

	/** Unequips items if the character can no longer meet the equip requirements. */
	UFUNCTION()
	virtual void OnMainJobChanged(UJobDefinition* JobDefinition);

	/** When an Item is removed from the InventoryManager, unequips the item if it is currently equipped. */
	UFUNCTION()
	virtual void OnItemRemovedFromContainer(const FItemInstance& ItemInstance);
	UFUNCTION()
	virtual void OnItemChangedInContainer(const FItemInstance& ItemInstance);

private:
	UPROPERTY(Replicated)
	FEquippedItemsContainer EquippedItemsContainer;

	/** Cached ASC from the owner. */
	UPROPERTY()
	TObjectPtr<UCrimAbilitySystemComponent> AbilitySystemComponent;
	
	/** The current level in the PrimaryAttributeSet using Channel0 as base. */
	int32 BaseLevel = 0;
	/** If true, allowed to Dual Wield one-handed weapons. */
	bool bDualWield = false;
	
	/** The level is set from the BaseLevel when Changing jobs is true. */
	int32 PreJobChangeLevel = -1;
	/** Set to true when the MainJob changes. Is set back to false after ChangingJobs. */
	bool bMainJobChanged = false;
	
	/** Cached CombatSystemComponent from the owner. */
	UPROPERTY()
	TObjectPtr<UCombatSystemComponent> CombatSystemComponent;

	/** Cached InventoryManagerComponent from the owner. */
	UPROPERTY()
	TObjectPtr<UInventoryManagerComponent> InventoryManagerComponent;
	
	/** Cached JobManagerComponent from the owner. */
	UPROPERTY()
	TObjectPtr<UJobSystemComponent> JobSystemComponent;
	
	/** Cached MainJob from the JSC. */
	UPROPERTY()
	TObjectPtr<UJobDefinition> MainJob;
	
	/** The predefined slots for Weapons. */
	FGameplayTagContainer WeaponEquipSlots;
	
	bool bChangingJobs = false;
	UFUNCTION()
	void OnChangingJobs(bool bInChangingJobs);

	/** Cached value of whether the owner is a simulated actor. */
	UPROPERTY()
	bool bCachedIsNetSimulated = false;
	void CacheIsNetSimulated();
	
	void SetInventoryManagerComponent(UInventoryManagerComponent* InventoryManager);
	void SetJobSystemComponent(UJobSystemComponent* JobSystem);
	void SetCombatSystemComponent(UCombatSystemComponent* CombatSystem);
	
	void TryBroadcastEquipmentManagerReady();
	bool bBroadcastedIsReady = false;
	
	void OnLevelAttributeChanged(const FOnAttributeChangeData& Data);
	void OnDualWieldTagCountChanged(FGameplayTag Tag, int32 NewCount);

	friend struct FEquippedItemsContainer;
	friend struct FEquippedItem;
	
	/** Searches for the ItemInstance from the allowed ItemContainers. */
	FItemInstance* FindItem(const FItemInstanceHandle& Handle) const;
	
	/** Loops through all equipped items and checking if the Owner still meets the equip requirements. */
	void UnequipInvalidItems();

	void EquipItemInternal(const FGameplayTag& EquipSlot, FItemInstance* ItemInstance);
	void UnequipItemInternal(const FGameplayTag& EquipSlot);

	FActiveGameplayEffectHandle ApplyEquipmentGameplayEffect(const TInstancedStruct<FItem>& Item);
	void ClearEquipmentManagerFromItemInstance(FItemInstance* ItemInstance);

	UFUNCTION(Server, Reliable)
	void Server_EquipItem(const FGameplayTag& EquipSlot, const FItemInstanceHandle& Handle);

	UFUNCTION(Server, Reliable)
	void Server_UnequipItem(const FGameplayTag& EquipSlot);
};

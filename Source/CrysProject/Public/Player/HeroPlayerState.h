// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "CrysPlayerState.h"
#include "GameplayTagAssetInterface.h"
#include "GenericTeamAgentInterface.h"
#include "InventorySystemInterface.h"
#include "AbilitySystem/Ability/Combat/CombatSystemInterface.h"
#include "EquipmentSystem/EquipmentSystemInterface.h"
#include "JobSystem/JobSystemInterface.h"
#include "HeroPlayerState.generated.h"

class UCombatSystemComponent;
class UHeroJobSystemComponent;
class UAbilitySet;
class UHitPointsComponent;
class UMovementAttributeSet;
class UJobAttributeSet;
class UDefenderAttributeSet;
class UAttackerAttributeSet;
class UAbilityAttributeSet;
class UPrimaryAttributeSet;
class UTacticalPointsAttributeSet;
class UManaPointsAttributeSet;
class UCrysHitPointsAttributeSet;
class UCrimAbilitySystemComponent;

/**
 * All hero's should use this player state. As this will restore and save data for the player.
 */
UCLASS()
class CRYSPROJECT_API AHeroPlayerState : public ACrysPlayerState, public IAbilitySystemInterface,
	public IInventorySystemInterface, public IGenericTeamAgentInterface, public IGameplayTagAssetInterface,
	public IJobSystemInterface, public IEquipmentSystemInterface, public ICombatSystemInterface
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCrimAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY()
	TObjectPtr<UCrysHitPointsAttributeSet> HitPointsAttributeSet;
	UPROPERTY()
	TObjectPtr<UManaPointsAttributeSet> ManaPointsAttributeSet;
	UPROPERTY()
	TObjectPtr<UTacticalPointsAttributeSet> TacticalPointsAttributeSet;
	UPROPERTY()
	TObjectPtr<UPrimaryAttributeSet> PrimaryAttributeSet;
	UPROPERTY()
	TObjectPtr<UAbilityAttributeSet> AbilityAttributeSet;
	UPROPERTY()
	TObjectPtr<UAttackerAttributeSet> AttackerAttributeSet;
	UPROPERTY()
	TObjectPtr<UDefenderAttributeSet> DefenderAttributeSet;
	UPROPERTY()
	TObjectPtr<UJobAttributeSet> JobAttributeSet;
	UPROPERTY()
	TObjectPtr<UMovementAttributeSet> MovementAttributeSet;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HitPointsComponent", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UHitPointsComponent> HitPointsComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HeroJobSystemComponent", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UHeroJobSystemComponent> HeroJobSystemComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CombatSystemComponent", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCombatSystemComponent> CombatSystemComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "InventoryManager", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInventoryManagerComponent> InventoryManagerComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EquipmentManager", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UEquipmentManagerComponent> EquipmentManagerComponent;
		
public:
	AHeroPlayerState();
	virtual void PostInitializeComponents() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	/** IAbilitySystemInterface */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	/** IInventorySystemInterface */
	virtual UInventoryManagerComponent* GetInventoryManagerComponent_Implementation() const override;
	/** IJobSystemInterface */
	virtual UJobSystemComponent* GetJobSystemComponent_Implementation() const override;
	/** IEquipmentManagerInterface */
	virtual UEquipmentManagerComponent* GetEquipmentManagerComponent_Implementation() const override;
	/** ICombatSystemInterface */
	virtual UCombatSystemComponent* GetCombatSystemComponent_Implementation() const override;
	
	// IGenericTeamAgentInterface
	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual void SetGenericTeamId(const FGenericTeamId& TeamID) override;
	
	// Implements IGameplayTagAssetInterface
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;
	virtual bool HasMatchingGameplayTag(FGameplayTag TagToCheck) const override;
	virtual bool HasAllMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const override;
	virtual bool HasAnyMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const override;
	
protected:

	
private:
	/** Using a value of 1 for Heroes. */
	UPROPERTY(VisibleAnywhere, Category = "Hero")
	FGenericTeamId GenericTeamId;
	
	/** Abilities, attributes, and gameplay effects to grant. */
	UPROPERTY(EditDefaultsOnly, Category = "Hero")
	TArray<TObjectPtr<UAbilitySet>> AbilitySets;
	
	void GrantAbilitySets();
};

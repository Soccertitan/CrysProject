// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "CrimTargetingSystemInterface.h"
#include "CrysCharacter.h"
#include "GameplayTagAssetInterface.h"
#include "GenericTeamAgentInterface.h"
#include "InventorySystemInterface.h"
#include "AbilitySystem/Ability/Combat/CombatSystemInterface.h"
#include "EquipmentSystem/EquipmentSystemInterface.h"
#include "JobSystem/JobSystemInterface.h"
#include "HeroCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UCrimAbilitySystemComponent;

/**
 * Player's will use this class to control their Hero.
 */
UCLASS()
class CRYSPROJECT_API AHeroCharacter : public ACrysCharacter, public IAbilitySystemInterface, public IGameplayTagAssetInterface,
	public IGenericTeamAgentInterface, public IInventorySystemInterface, public IJobSystemInterface, public IEquipmentSystemInterface,
	public ICombatSystemInterface, public ICrimTargetingSystemInterface
{
	GENERATED_BODY()
	
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;

public:
	AHeroCharacter(const FObjectInitializer& ObjectInitializer);
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual UInventoryManagerComponent* GetInventoryManagerComponent_Implementation() const override;
	virtual UJobSystemComponent* GetJobSystemComponent_Implementation() const override;
	virtual UEquipmentManagerComponent* GetEquipmentManagerComponent_Implementation() const override;
	virtual UCombatSystemComponent* GetCombatSystemComponent_Implementation() const override;
	virtual UCrimTargetingSystemComponent* GetCrimTargetingSystemComponent_Implementation() const override;
	
	// IGenericTeamAgentInterface
	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual void SetGenericTeamId(const FGenericTeamId& TeamID) override;
	
	// Implements IGameplayTagAssetInterface
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;
	virtual bool HasMatchingGameplayTag(FGameplayTag TagToCheck) const override;
	virtual bool HasAllMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const override;
	virtual bool HasAnyMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const override;
	
protected:
	virtual void InitAbilitySystem();
	
private:
	/** Cached ASC from the PlayerState. */
	UPROPERTY()
	TObjectPtr<UCrimAbilitySystemComponent> AbilitySystemComponent;
};

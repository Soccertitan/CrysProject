// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "CrysCharacter.h"
#include "GameplayTagAssetInterface.h"
#include "GenericTeamAgentInterface.h"
#include "NonPlayerCharacter.generated.h"

class UJobAttributeSet;
class UAbilitySet;
class UHitPointsComponent;
class UMovementAttributeSet;
class UDefenderAttributeSet;
class UAttackerAttributeSet;
class UAbilityAttributeSet;
class UPrimaryAttributeSet;
class UTacticalPointsAttributeSet;
class UManaPointsAttributeSet;
class UCrysHitPointsAttributeSet;
class UCrimAbilitySystemComponent;

UCLASS()
class CRYSPROJECT_API ANonPlayerCharacter : public ACrysCharacter, public IAbilitySystemInterface,
	public IGenericTeamAgentInterface, public IGameplayTagAssetInterface
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

public:
	ANonPlayerCharacter(const FObjectInitializer& ObjectInitializer);
	virtual void PostInitializeComponents() override;
	
	/** IAbilitySystemInterface */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	// IGenericTeamAgentInterface
	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual void SetGenericTeamId(const FGenericTeamId& TeamID) override;
	
	// Implements IGameplayTagAssetInterface
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;
	virtual bool HasMatchingGameplayTag(FGameplayTag TagToCheck) const override;
	virtual bool HasAllMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const override;
	virtual bool HasAnyMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const override;
	
private:
	/** A value of 1 is a hero. Use 2 for enemies. */
	UPROPERTY(EditAnywhere, Category = "Character")
	FGenericTeamId GenericTeamId;
	
	/** Abilities, attributes, and gameplay effects to grant. */
	UPROPERTY(EditAnywhere, Category = "Character")
	TArray<TObjectPtr<UAbilitySet>> AbilitySets;
	
	void GrantAbilitySets();
};

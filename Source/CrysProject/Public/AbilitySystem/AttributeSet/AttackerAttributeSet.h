// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Attribute/CrimAttributeSet.h"
#include "AttackerAttributeSet.generated.h"

/**
 * Attributes are compared to a Defenders attributes to determine damage.
 */
UCLASS()
class CRYSPROJECT_API UAttackerAttributeSet : public UCrimAttributeSet
{
	GENERATED_BODY()
	
public:
	UAttackerAttributeSet();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	ATTRIBUTE_ACCESSORS(ThisClass, Attack);
	ATTRIBUTE_ACCESSORS(ThisClass, Accuracy);
	ATTRIBUTE_ACCESSORS(ThisClass, DefencePierce);
	ATTRIBUTE_ACCESSORS(ThisClass, AttackDefenceRatioCap);
	ATTRIBUTE_ACCESSORS(ThisClass, HitChance);
	ATTRIBUTE_ACCESSORS(ThisClass, CriticalHitChance);
	ATTRIBUTE_ACCESSORS(ThisClass, CriticalHitBonus);
	
	ATTRIBUTE_ACCESSORS(ThisClass, AutoAttackDelay);
	
protected:
	virtual void ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const override;
	
	UFUNCTION()
	void OnRep_Attack(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	void OnRep_AutoAttackDelay(const FGameplayAttributeData& OldValue);
	
private:
	/** Attack Power */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Attack, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Attack;

	/** Increases the chance for abilities to be successful. */
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Accuracy;
	
	/** Defense * (1 - DefensePierce). Ignores a percentage of the defenders defence. */
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData DefencePierce;
	
	/** Limits the damage multiplier when Attack/Defence. */
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AttackDefenceRatioCap;
	
	/** Increases the chance to hit. */
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData HitChance;
	
	/** Increases the chance to critically hit */
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData CriticalHitChance;

	/** Adds to the Attack/Defense ratio when a critical hit occurs. */
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData CriticalHitBonus;

	/** The amount of time between auto attacks in seconds. */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_AutoAttackDelay, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AutoAttackDelay;
};

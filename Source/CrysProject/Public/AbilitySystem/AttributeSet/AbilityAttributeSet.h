// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Attribute/CrimAttributeSet.h"
#include "AbilityAttributeSet.generated.h"

/**
 * The various attributes that can affect abilities and effects.
 */
UCLASS()
class CRYSPROJECT_API UAbilityAttributeSet : public UCrimAttributeSet
{
	GENERATED_BODY()
	
public:
	UAbilityAttributeSet();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	ATTRIBUTE_ACCESSORS(ThisClass, PotencyMultiplier);
	ATTRIBUTE_ACCESSORS(ThisClass, CastSpeedMultiplier);
	ATTRIBUTE_ACCESSORS(ThisClass, AbilityCooldownMultiplier);
	ATTRIBUTE_ACCESSORS(ThisClass, AbilityCostMultiplier);
	ATTRIBUTE_ACCESSORS(ThisClass, EnmityMultiplier);
	
	ATTRIBUTE_ACCESSORS(ThisClass, WeaponSkill);
	ATTRIBUTE_ACCESSORS(ThisClass, MagicSkill);
	
protected:
	virtual void ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const override;
	
	UFUNCTION()
	void OnRep_CastSpeedMultiplier(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	void OnRep_WeaponSkill(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_MagicSkill(const FGameplayAttributeData& OldValue);
	
private:
	/** Potency. A generic attribute for multiplying the effectiveness of abilities. */
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData PotencyMultiplier;
	
	/** The multiplier for casting abilities. */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_CastSpeedMultiplier, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData CastSpeedMultiplier;

	/** Affects the cooldown of abilities. */
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AbilityCooldownMultiplier;

	/** Affects the cost of abilities. */
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AbilityCostMultiplier;

	/** Adjust the amount of threat generated towards enemies. */
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData EnmityMultiplier;
	
	/** Boost the effectiveness of weapons. */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_WeaponSkill, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData WeaponSkill;
	/** Boost the effectiveness of magic. */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MagicSkill, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MagicSkill;
};

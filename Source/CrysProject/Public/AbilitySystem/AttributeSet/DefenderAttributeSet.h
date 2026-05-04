// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Attribute/CrimAttributeSet.h"
#include "DefenderAttributeSet.generated.h"

/**
 * Protects the character by mitigating or avoiding damage.
 */
UCLASS()
class CRYSPROJECT_API UDefenderAttributeSet : public UCrimAttributeSet
{
	GENERATED_BODY()
	
public:
	UDefenderAttributeSet();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	ATTRIBUTE_ACCESSORS(ThisClass, Defence);
	ATTRIBUTE_ACCESSORS(ThisClass, Evasion);
	ATTRIBUTE_ACCESSORS(ThisClass, EvasionSkill);
	ATTRIBUTE_ACCESSORS(ThisClass, Resistance);
	
	ATTRIBUTE_ACCESSORS(ThisClass, GuardSkill);
	ATTRIBUTE_ACCESSORS(ThisClass, GuardChance);
	ATTRIBUTE_ACCESSORS(ThisClass, GuardHalfAngle);
	ATTRIBUTE_ACCESSORS(ThisClass, GuardDamageReduction);
	
	ATTRIBUTE_ACCESSORS(ThisClass, ParrySkill);
	ATTRIBUTE_ACCESSORS(ThisClass, ParryChance);
	ATTRIBUTE_ACCESSORS(ThisClass, ParryHalfAngle);
	
	ATTRIBUTE_ACCESSORS(ThisClass, ShieldSkill);
	ATTRIBUTE_ACCESSORS(ThisClass, BlockChance);
	ATTRIBUTE_ACCESSORS(ThisClass, BlockHalfAngle);
	ATTRIBUTE_ACCESSORS(ThisClass, BlockDamageReduction);
	
protected:
	virtual void ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const override;
	
	UFUNCTION()
	void OnRep_Defence(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_EvasionSkill(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_Resistance(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	void OnRep_GuardSkill(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	void OnRep_ParrySkill(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	void OnRep_ShieldSkill(const FGameplayAttributeData& OldValue);
	
private:
	/** Defense Power boosted by Vitality and Mind. */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Defence, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Defence;
	/** Increases chance to avoid damage and status effects with Agility and Charisma. */
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Evasion;
	/** Boost the likelihood of evading attacks. */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_EvasionSkill, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData EvasionSkill;
	/** The higher the resistance, less damage taken at a percentage level. A value greater than 1 will cause healing. */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Resistance, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Resistance;
	
	/** Boost the likelihood of guarding. */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_GuardSkill, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData GuardSkill;
	/** The base chance of guarding. */
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData GuardChance;
	/** The half angle in front of the actor valid for guarding. */
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData GuardHalfAngle;
	/** The reduction in damage from guarding. */
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData GuardDamageReduction;
	
	/** Boost the likelihood of parrying. */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_ParrySkill, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData ParrySkill;
	/** The likelihood of parrying. */
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData ParryChance;
	/** The front angle valid for parrying. */
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData ParryHalfAngle;
	
	/** Boost the likelihood of blocking. */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_ShieldSkill, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData ShieldSkill;
	/** The likelihood of blocking. */
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData BlockChance;
	/** The front angle valid for blocking. */
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData BlockHalfAngle;
	/** The percent reduction in taking damage from blocking. */
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData BlockDamageReduction;
};

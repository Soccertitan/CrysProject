// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Attribute/CrimAttributeSet.h"
#include "PrimaryAttributeSet.generated.h"

/**
 * Contains the basic attributes that can govern other attributes.
 */
UCLASS()
class CRYSPROJECT_API UPrimaryAttributeSet : public UCrimAttributeSet
{
	GENERATED_BODY()
	
public:
	UPrimaryAttributeSet();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	ATTRIBUTE_ACCESSORS(ThisClass, Level);
	ATTRIBUTE_ACCESSORS(ThisClass, Strength);
	ATTRIBUTE_ACCESSORS(ThisClass, Vitality);
	ATTRIBUTE_ACCESSORS(ThisClass, Dexterity);
	ATTRIBUTE_ACCESSORS(ThisClass, Agility);
	ATTRIBUTE_ACCESSORS(ThisClass, Intelligence);
	ATTRIBUTE_ACCESSORS(ThisClass, Mind);
	ATTRIBUTE_ACCESSORS(ThisClass, Charisma);

protected:
	virtual void ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const override;

	UFUNCTION()
	void OnRep_Level(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_Vitality(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_Dexterity(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_Agility(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_Intelligence(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_Mind(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_Charisma(const FGameplayAttributeData& OldValue);

private:
	/** The level of the character */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Level, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Level;
	/** Physical attack power. */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Strength, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Strength;
	/** Physical defense power and Max HP */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Vitality, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Vitality;
	/** Increases accuracy and critical hit rate for abilities. */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Dexterity, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Dexterity;
	/** Increases evasion from abilities */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Agility, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Agility;
	/** Magic attack power */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Intelligence, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Intelligence;
	/** Magic defense power and Max MP */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Mind, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Mind;
	/** Affects different abilities in different ways. */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Charisma, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Charisma;
};

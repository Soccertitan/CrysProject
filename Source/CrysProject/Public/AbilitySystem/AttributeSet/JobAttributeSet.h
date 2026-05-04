// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Attribute/CrimAttributeSet.h"
#include "JobAttributeSet.generated.h"

/**
 * Directly affects the PrimaryAttribute's base attributes.
 */
UCLASS()
class CRYSPROJECT_API UJobAttributeSet : public UCrimAttributeSet
{
	GENERATED_BODY()
	
public:
	UJobAttributeSet();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	ATTRIBUTE_ACCESSORS(ThisClass, MainJobLevel);
	ATTRIBUTE_ACCESSORS(ThisClass, SubJobLevel);
	ATTRIBUTE_ACCESSORS(ThisClass, SubJobEffectiveness);
	
protected:
	virtual void ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const override;
	
	UFUNCTION()
	void OnRep_MainJobLevel(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_SubJobLevel(const FGameplayAttributeData& OldValue);
	
private:
	/** MainJob Rank */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MainJobLevel, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MainJobLevel;
	
	/** SubJob Rank */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_SubJobLevel, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData SubJobLevel;
	
	/** A multiplier to the SubJob's stats. */
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData SubJobEffectiveness;
};

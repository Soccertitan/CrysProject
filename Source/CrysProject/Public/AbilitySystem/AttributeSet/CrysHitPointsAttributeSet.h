// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "Attribute/HitPointsAttributeSet.h"
#include "CrysHitPointsAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class CRYSPROJECT_API UCrysHitPointsAttributeSet : public UHitPointsAttributeSet
{
	GENERATED_BODY()
	
public:
	UCrysHitPointsAttributeSet();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	ATTRIBUTE_ACCESSORS(ThisClass, BarrierPoints);
	
protected:
	UFUNCTION()
	void OnRep_BarrierPoints(const FGameplayAttributeData& OldValue);
	
	virtual void ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void HandleDamage(const FGameplayEffectModCallbackData& Data, float Magnitude) override;
	
private:
	/** Damage will be inflicted to this attribute before being passed onto Health. */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BarrierPoints, Category = "Attribute", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData BarrierPoints;
};

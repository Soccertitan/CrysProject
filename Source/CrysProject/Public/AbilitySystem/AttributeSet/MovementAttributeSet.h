// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Attribute/CrimAttributeSet.h"
#include "MovementAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class CRYSPROJECT_API UMovementAttributeSet : public UCrimAttributeSet
{
	GENERATED_BODY()
	
public:
	UMovementAttributeSet();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	
	ATTRIBUTE_ACCESSORS(ThisClass, MovementSpeedMultiplier);
	
protected:
	virtual void ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const override;
		
	UFUNCTION()
	virtual void OnRep_MovementSpeedMultiplier(const FGameplayAttributeData& OldValue);

private:
	UPROPERTY(BlueprintReadOnly, Category = "Attribute", ReplicatedUsing=OnRep_MovementSpeedMultiplier, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MovementSpeedMultiplier;
};

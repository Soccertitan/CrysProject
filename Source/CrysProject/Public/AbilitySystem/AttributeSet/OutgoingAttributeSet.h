// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Attribute/CrimAttributeSet.h"
#include "OutgoingAttributeSet.generated.h"

/**
 * Are not replicated used for sending a base value to a GE.
 */
UCLASS()
class CRYSPROJECT_API UOutgoingAttributeSet : public UCrimAttributeSet
{
	GENERATED_BODY()
	
public:
	UOutgoingAttributeSet();
	
	ATTRIBUTE_ACCESSORS(ThisClass, OutPotency);
	ATTRIBUTE_ACCESSORS(ThisClass, OutProbability);
	
protected:
	virtual void ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const override;
	
private:
	/** Potency. A generic attribute for base effectiveness of abilities. */
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData OutPotency;
	
	/** Base likelihood of an ability hitting or an effect happening. */
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData OutProbability;
};

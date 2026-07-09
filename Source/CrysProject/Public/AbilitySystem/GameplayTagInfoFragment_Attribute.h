// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect/Component/GameplayEffectTextAttributes.h"
#include "System/CrysGameplayTagRelationship.h"

#include "GameplayTagInfoFragment_Attribute.generated.h"


USTRUCT(DisplayName = "Attribute")
struct FGameplayTagInfoFragment_Attribute : public FCrysGameplayTagInfoFragment
{
	GENERATED_BODY()
	
	/** The text that is used when generating the AttributeText in a GameplayEffect. */
	UPROPERTY(EditAnywhere, meta = (MultiLine))
	FText DisplayText;
	
	/** Set to true to display the value as a percent. i.e. 0.12 will display as 12% */
	UPROPERTY(EditAnywhere)
	bool bDisplayValueAsPercent = false;
	
	/** The attribute to query from the ASC. */
	UPROPERTY(EditAnywhere)
	FGameplayAttribute GameplayAttribute;

	/** Evaluates the GameplayAttribute with source tags. */
	UPROPERTY(EditAnywhere)
	FGameplayTagContainer SourceTags;
};

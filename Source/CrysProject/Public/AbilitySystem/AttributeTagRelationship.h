// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AttributeTagRelationship.generated.h"

/** Defines the relationship between a GameplayTag and attribute information. */
USTRUCT()
struct FAttributeTagInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (Categories = "Attribute"))
	FGameplayTag AttributeTag;
	
	/** Set to true to display the value as a percent. i.e. 0.12 will display as 12.*/
	UPROPERTY(EditAnywhere)
	bool bDisplayValueAsPercent = false;

	/** The attribute to query from the ASC. */
	UPROPERTY(EditAnywhere)
	FGameplayAttribute GameplayAttribute;

	/** Evaluates the GameplayAttribute with source tags. */
	UPROPERTY(EditAnywhere)
	FGameplayTagContainer SourceTags;

	bool IsValid() const;

	friend bool operator==(const FAttributeTagInfo& X, const FAttributeTagInfo& Y)
	{
		return X.AttributeTag == Y.AttributeTag;
	}
	friend bool operator!=(const FAttributeTagInfo& X, const FAttributeTagInfo& Y)
	{
		return X.AttributeTag != Y.AttributeTag;
	}
};

/**
 * Maps data about an Attribute gameplay tag to its properties for UI purposes.
 */
UCLASS(Const)
class CRYSPROJECT_API UAttributeTagRelationship : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	FAttributeTagInfo FindAttributeTagInfo(const FGameplayTag& AttributeTag, bool bLogNotFound = false) const;
	
	FAttributeTagInfo FindAttributeTagInfo(const FGameplayAttribute& GameplayAttribute, const FGameplayTagContainer& SourceTags, bool bLogNotFound = false) const;

protected:
	UPROPERTY(EditDefaultsOnly, meta = (NoResetToDefault, NoElementDuplicate, ForceInlineRow, TitleProperty = "{AttributeTag}"))
	TArray<FAttributeTagInfo> AttributeTags;
};

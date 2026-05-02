// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "UITagRelationship.generated.h"

/** Defines the relationship between a GameplayTag and UI information. */
USTRUCT()
struct FUITagInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	FGameplayTag Tag;
	
	UPROPERTY(EditAnywhere)
	FText Name;

	UPROPERTY(EditAnywhere)
	FText ShortName;
	
	UPROPERTY(EditAnywhere, meta = (MultiLine=true))
	FText Description;
	
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UTexture2D> Icon;
	
	friend bool operator==(const FUITagInfo& X, const FUITagInfo& Y)
	{
		return X.Tag == Y.Tag;
	}
	friend bool operator!=(const FUITagInfo& X, const FUITagInfo& Y)
	{
		return X.Tag != Y.Tag;
	}
};

/**
 * Describes the data about a tag in relation to the UI.
 */
UCLASS(const)
class CRYSPROJECT_API UUITagRelationship : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	FUITagInfo FindUITagInfo(UPARAM(ref) const FGameplayTag& Tag, bool bLogNotFound = false) const;

protected:
	UPROPERTY(EditDefaultsOnly, meta = (NoResetToDefault, NoElementDuplicate, ForceInlineRow, TitleProperty = "{Tag}"))
	TArray<FUITagInfo> UITags;
};

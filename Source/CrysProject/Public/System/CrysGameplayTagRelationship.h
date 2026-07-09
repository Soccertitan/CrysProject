// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "StructUtils/InstancedStruct.h"
#include "CrysGameplayTagRelationship.generated.h"

/** Base struct that contains information used in the TagInfo Struct.*/
USTRUCT()
struct FCrysGameplayTagInfoFragment
{
	GENERATED_BODY()
	
};

/** Defines the relationship between a GameplayTag and misc information. */
USTRUCT()
struct FCrysGameplayTagInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	FGameplayTag Tag;
	
	UPROPERTY(EditAnywhere, meta = (ExcludeBaseStruct))
	TArray<TInstancedStruct<FCrysGameplayTagInfoFragment>> Fragments;
	
	template<typename T> requires std::derived_from<T, FCrysGameplayTagInfoFragment>
	const T* FindFragmentByType() const;
	
	friend bool operator==(const FCrysGameplayTagInfo& X, const FCrysGameplayTagInfo& Y)
	{
		return X.Tag == Y.Tag;
	}
	friend bool operator!=(const FCrysGameplayTagInfo& X, const FCrysGameplayTagInfo& Y)
	{
		return X.Tag != Y.Tag;
	}
	friend bool operator<(const FCrysGameplayTagInfo& X, const FCrysGameplayTagInfo& Y)
	{
		return X.Tag < Y.Tag;
	}
	
	bool IsValid() const;
};

template <typename T> requires std::derived_from<T, FCrysGameplayTagInfoFragment>
const T* FCrysGameplayTagInfo::FindFragmentByType() const
{
	for (const TInstancedStruct<FCrysGameplayTagInfoFragment>& Fragment : Fragments)
	{
		if (const T* Ptr = Fragment.GetPtr<T>())
		{
			return Ptr;
		}
	}
	return nullptr;
}

/**
 * Contains a collection of GameplayTags with associated data.
 */
UCLASS(Const)
class CRYSPROJECT_API UCrysGameplayTagRelationship : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	FCrysGameplayTagInfo FindInfo(UPARAM(ref) const FGameplayTag& Tag, bool bLogNotFound = false) const;
	
	UPROPERTY(EditDefaultsOnly, meta = (NoResetToDefault, ForceInlineRow, TitleProperty = "{Tag}"))
	TArray<FCrysGameplayTagInfo> GameplayTagInfos;
	
#if WITH_EDITORONLY_DATA
	/** Sorting the UITags based on Tag. */
	virtual void PreSave(FObjectPreSaveContext SaveContext) override;
#endif
};

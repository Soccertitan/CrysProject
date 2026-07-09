// Fill out your copyright notice in the Description page of Project Settings.


#include "CrysProject/Public/AbilitySystem/AttributeTagRelationship.h"

#include "CrysLogChannels.h"
#include "UObject/ObjectSaveContext.h"

bool FAttributeTagInfo::IsValid() const
{
	if (AttributeTag.IsValid() && GameplayAttribute.IsValid())
	{
		return true;
	}
	return false;
}

FAttributeTagInfo UAttributeTagRelationship::FindAttributeTagInfo(const FGameplayTag& AttributeTag, bool bLogNotFound) const
{
	for (const FAttributeTagInfo& Item : AttributeTags)
	{
		if (Item.AttributeTag == AttributeTag)
		{
			return Item;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogCrys, Error, TEXT("Cannot find info for AttributeTag [%s] in UAttributeTagRelationship [%s]"), *AttributeTag.ToString(), *GetNameSafe(this));
	}
	return FAttributeTagInfo();
}

FAttributeTagInfo UAttributeTagRelationship::FindAttributeTagInfo(const FGameplayAttribute& GameplayAttribute, const FGameplayTagContainer& SourceTags, bool bLogNotFound) const
{
	for (const FAttributeTagInfo& Item : AttributeTags)
	{
		if (Item.GameplayAttribute == GameplayAttribute && Item.SourceTags == SourceTags)
		{
			return Item;
		}
	}
	
	if (bLogNotFound)
	{
		UE_LOG(LogCrys, Error, TEXT("Cannot find info for GameplayAttribute [%s] and SourceTags [%s] in UAttributeTagRelationship [%s]"), *GameplayAttribute.AttributeName, *SourceTags.ToString(), *GetNameSafe(this));
	}
	return FAttributeTagInfo();
}

#if WITH_EDITORONLY_DATA
void UAttributeTagRelationship::PreSave(FObjectPreSaveContext SaveContext)
{
	Super::PreSave(SaveContext);
	
	Algo::Sort(AttributeTags, [](const FAttributeTagInfo& A, const FAttributeTagInfo& B)
	{
		return A.AttributeTag < B.AttributeTag;
	});
}
#endif

// Fill out your copyright notice in the Description page of Project Settings.


#include "CrysProject/Public/AbilitySystem/AttributeTagRelationship.h"

#include "CrysLogChannels.h"

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

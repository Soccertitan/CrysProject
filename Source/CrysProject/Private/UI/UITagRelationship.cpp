// Copyright Soccertitan 2026


#include "UI/UITagRelationship.h"

#include "CrysLogChannels.h"

FUITagInfo UUITagRelationship::FindUITagInfo(const FGameplayTag& Tag, bool bLogNotFound) const
{
	for (const FUITagInfo& Item : UITags)
	{
		if (Item.Tag == Tag)
		{
			return Item;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogCrys, Error, TEXT("Cannot find info for Tag [%s] in UITagRelationship [%s]"), *Tag.ToString(), *GetNameSafe(this));
	}
	return FUITagInfo();
}

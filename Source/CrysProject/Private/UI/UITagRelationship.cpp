// Copyright Soccertitan 2026


#include "UI/UITagRelationship.h"

#include "CrysLogChannels.h"
#include "UObject/ObjectSaveContext.h"

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

#if WITH_EDITORONLY_DATA
void UUITagRelationship::PreSave(FObjectPreSaveContext SaveContext)
{
	Super::PreSave(SaveContext);
	
	UITags.Sort([](const FUITagInfo& A, const FUITagInfo& B)
	{
		return A.Tag < B.Tag;
	});
}
#endif


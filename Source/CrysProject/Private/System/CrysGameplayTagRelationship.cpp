// Copyright Soccertitan 2026


#include "System/CrysGameplayTagRelationship.h"

#include "CrysLogChannels.h"
#include "UObject/ObjectSaveContext.h"

bool FCrysGameplayTagInfo::IsValid() const
{
	return Tag.IsValid();
}

const FCrysGameplayTagInfo* UCrysGameplayTagRelationship::FindInfo(const FGameplayTag& Tag, bool bLogNotFound) const
{
	for (const FCrysGameplayTagInfo& Item : GameplayTagInfos)
	{
		if (Item.Tag == Tag)
		{
			return &Item;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogCrys, Error, TEXT("Cannot find info for Tag [%s] in CrysGameplayTagRelationship [%s]"), *Tag.ToString(), *GetNameSafe(this));
	}
	return nullptr;
}

void UCrysGameplayTagRelationship::PreSave(FObjectPreSaveContext SaveContext)
{
	Super::PreSave(SaveContext);
	
	GameplayTagInfos.Sort();
}

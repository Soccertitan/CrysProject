// Copyright Soccertitan 2026


#include "JobSystem/HeroJobTypes.h"

#include "JobSystem/HeroJobSystemComponent.h"
#include "JobSystem/JobDefinition.h"
#include "JobSystem/JobSystemBlueprintFunctionLibrary.h"


void FJobProgressItem::PostReplicatedAdd(const struct FJobProgressContainer& InSerializer)
{
	InSerializer.Owner->OnJobProgressUpdatedDelegate.Broadcast(*this);
}

void FJobProgressItem::PostReplicatedChange(const FJobProgressContainer& InSerializer)
{
	InSerializer.Owner->OnJobProgressUpdatedDelegate.Broadcast(*this);
}

bool FJobProgressItem::IsValid() const
{
	if (Job.IsNull())
	{
		return false;
	}
	return true;
}

void FJobProgressContainer::AddJobProgressItem(const FJobProgressItem& NewItem)
{
	FJobProgressItem& Item = Items.AddDefaulted_GetRef();
	Item.Job = NewItem.Job;
	Item.Level = NewItem.Level;
	Item.Experience = NewItem.Experience;
	
	MarkItemDirty(Item);
	Owner->OnJobProgressUpdatedDelegate.Broadcast(NewItem);
}

void FJobProgressContainer::AddExperience(const UJobDefinition* Job, const FScalableFloat& ExperienceRequirement, int32 Experience)
{
	for (FJobProgressItem& Item : Items)
	{
		if (Item.Job == Job)
		{
			AddExperienceInternal(Item, ExperienceRequirement, Experience);
			return;
		}
	}

	FJobProgressItem& NewItem = Items.AddDefaulted_GetRef();
	NewItem.Job = Job->GetPathName();
	NewItem.Level = 1;
	AddExperienceInternal(NewItem, ExperienceRequirement, Experience);
}

FJobProgressItem* FJobProgressContainer::FindJobProgressItem(const TSoftObjectPtr<UJobDefinition>& Job) const
{
	for (const FJobProgressItem& Item : Items)
	{
		if (Item.Job == Job)
		{
			return const_cast<FJobProgressItem*>(&Item);
		}
	}
	return nullptr;
}

void FJobProgressContainer::Reset()
{
	Items.Empty();
	MarkArrayDirty();
}

void FJobProgressContainer::AddExperienceInternal(FJobProgressItem& Item, const FScalableFloat& ExperienceRequirement, int32 Experience)
{
	const int32 OldLevel = Item.Level;
	const bool bLeveledUp = UJobSystemBlueprintFunctionLibrary::AddExperience(
		Item.Level, Item.Experience, Experience, ExperienceRequirement, Owner->GetHeroJobSystemData().MaxJobLevel);
	MarkItemDirty(Item);
	Owner->OnJobProgressUpdatedDelegate.Broadcast(Item);
	if (bLeveledUp)
	{
		Owner->OnJobLevelUpInternal(Item, OldLevel);
	}
}

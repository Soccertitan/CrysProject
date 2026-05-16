// Copyright Soccertitan 2026


#include "JobSystem/UI/HeroJobViewModel.h"

#include "JobSystem/JobDefinition.h"
#include "JobSystem/UI/JobViewModel.h"

int32 UHeroJobViewModel::GetExperienceRequiredUntilNextLevel() const
{
	if (JobViewModel)
	{
		const int32 RequiredExp = JobViewModel->GetJobDefinition()->ExperienceRequirement.GetValueAtLevel(JobProgressItem.Level);
		return FMath::Max(RequiredExp - JobProgressItem.Experience, 0);
	}
	return 0;
}

float UHeroJobViewModel::GetPercentageTowardsNextLevel() const
{
	if (JobViewModel)
	{
		const float PreviousLevelRequirement = JobViewModel->GetJobDefinition()->ExperienceRequirement.GetValueAtLevel(JobProgressItem.Level - 1);
		const float NextLevelRequirement = JobViewModel->GetJobDefinition()->ExperienceRequirement.GetValueAtLevel(JobProgressItem.Level);
		
		const float ExperienceIntoNextLevel = JobProgressItem.Experience - PreviousLevelRequirement;
		const float TotalExperienceRequired = NextLevelRequirement - PreviousLevelRequirement;
		
		if (TotalExperienceRequired > 0)
		{
			return FMath::Clamp(ExperienceIntoNextLevel / TotalExperienceRequired, 0.f, 1.f);
		}
	}

	return 0.f;
}

void UHeroJobViewModel::SetJobViewModel(UJobViewModel* Job)
{
	UE_MVVM_SET_PROPERTY_VALUE(JobViewModel, Job);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetExperienceRequiredUntilNextLevel);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetPercentageTowardsNextLevel);
}

void UHeroJobViewModel::SetJobProgress(const FJobProgressItem& JobProgress)
{
	JobProgressItem = JobProgress;
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetLevel);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetExperience);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetExperienceRequiredUntilNextLevel);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetPercentageTowardsNextLevel);
	SetIsJobAvailable(true);
}

void UHeroJobViewModel::SetIsJobAvailable(bool InValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(bJobAvailable, InValue);
}

void UHeroJobViewModel::SetIsMainJob(bool InValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(bMainJob, InValue);
}

void UHeroJobViewModel::SetIsSubJob(bool InValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(bSubJob, InValue);
}

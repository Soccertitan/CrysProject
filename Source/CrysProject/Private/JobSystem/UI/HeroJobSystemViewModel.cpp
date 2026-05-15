// Copyright Soccertitan 2026


#include "JobSystem/UI/HeroJobSystemViewModel.h"

#include "JobSystem/HeroJobSystemComponent.h"
#include "JobSystem/JobDefinition.h"
#include "JobSystem/UI/HeroJobViewModel.h"
#include "JobSystem/UI/JobViewModel.h"


void UHeroJobSystemViewModel::SetHeroJobManagerComponent(UHeroJobSystemComponent* HeroJobSystem)
{
	if (HeroJobSystemComponent == HeroJobSystem)
	{
		return;
	}
	
	if (HeroJobSystemComponent)
	{
		HeroJobSystemComponent->OnMainJobChangedDelegate.RemoveAll(this);
		HeroJobSystemComponent->OnSubJobChangedDelegate.RemoveAll(this);
		HeroJobSystemComponent->OnTrySetJobDelegate.RemoveAll(this);
		HeroJobSystemComponent->OnJobProgressUpdatedDelegate.RemoveAll(this);
		HeroJobSystemComponent->OnHeroJobSystemDataUpdatedDelegate.RemoveAll(this);
	}
	
	HeroJobSystemComponent = HeroJobSystem;
	
	if (HeroJobSystemComponent)
	{
		HeroJobSystemComponent->OnMainJobChangedDelegate.AddUniqueDynamic(this, &UHeroJobSystemViewModel::OnMainJobChanged);
		HeroJobSystemComponent->OnSubJobChangedDelegate.AddUniqueDynamic(this, &UHeroJobSystemViewModel::OnSubJobChanged);
		HeroJobSystemComponent->OnTrySetJobDelegate.AddUniqueDynamic(this, &UHeroJobSystemViewModel::OnTrySetJob);
		HeroJobSystemComponent->OnJobProgressUpdatedDelegate.AddUniqueDynamic(this, &UHeroJobSystemViewModel::OnJobProgressUpdated);
		HeroJobSystemComponent->OnHeroJobSystemDataUpdatedDelegate.AddUniqueDynamic(this, &UHeroJobSystemViewModel::OnHeroJobSystemDataUpdated);
		
		InitializeStartingData();
	}
}

UHeroJobViewModel* UHeroJobSystemViewModel::FindOrCreateHeroJobViewModel(TSoftObjectPtr<UJobDefinition> JobDefinition)
{
	if (!JobDefinition.IsNull())
	{
		for (TObjectPtr<UHeroJobViewModel>& ViewModel : HeroJobViewModels)
		{
			if (ViewModel->JobViewModel->JobDefinition == JobDefinition.Get())
			{
				return ViewModel;
			}
		}
		
		return CreateHeroJobViewModel(JobDefinition);
	}
	return nullptr;
}

int32 UHeroJobSystemViewModel::GetExperienceRequiredUntilNextLevel() const
{
	if (HeroJobSystemComponent && HeroJobSystemComponent->GetRace())
	{
		UJobDefinition* RaceDefinition = HeroJobSystemComponent->GetRace();
		const int32 RequiredExp = RaceDefinition->ExperienceRequirement.GetValueAtLevel(HeroJobSystemData.Level);
		return FMath::Max(RequiredExp - HeroJobSystemData.Experience, 0);
	}
	return 0;
}

float UHeroJobSystemViewModel::GetPercentageTowardsNextLevel() const
{
	if (HeroJobSystemComponent && HeroJobSystemComponent->GetRace())
	{
		UJobDefinition* RaceDefinition = HeroJobSystemComponent->GetRace();
		const float PreviousLevelRequirement = RaceDefinition->ExperienceRequirement.GetValueAtLevel(HeroJobSystemData.Level - 1);
		const float NextLevelRequirement = RaceDefinition->ExperienceRequirement.GetValueAtLevel(HeroJobSystemData.Level);
		
		const float ExperienceIntoNextLevel = HeroJobSystemData.Experience - PreviousLevelRequirement;
		const float TotalExperienceRequired = NextLevelRequirement - PreviousLevelRequirement;
		
		if (TotalExperienceRequired > 0)
		{
			return FMath::Clamp(ExperienceIntoNextLevel / TotalExperienceRequired, 0.f, 1.f);
		}
	}

	return 0.f;
}

void UHeroJobSystemViewModel::TrySetJobs(UHeroJobViewModel* InMainJobViewModel, UHeroJobViewModel* InSubJobViewModel)
{
	if (!InMainJobViewModel || !HeroJobSystemComponent || bSwitchingJobs)
	{
		return;
	}
	
	UJobDefinition* MainJob = InMainJobViewModel->GetJobViewModel()->JobDefinition;
	UJobDefinition* SubJob = InSubJobViewModel ? InSubJobViewModel->GetJobViewModel()->JobDefinition : nullptr;
	
	UE_MVVM_SET_PROPERTY_VALUE(bSwitchingJobs, true);
	HeroJobSystemComponent->TrySetJobs(MainJob, SubJob);
}

void UHeroJobSystemViewModel::TrySetMainJob(UHeroJobViewModel* JobViewModel)
{
	if (HeroJobSystemComponent && JobViewModel && bSwitchingJobs == false)
	{
		if (SubJobViewModel->GetJobViewModel()->JobDefinition == JobViewModel->GetJobViewModel()->JobDefinition)
		{
			TrySetJobs(JobViewModel, MainJobViewModel);
		}
		else
		{
			TrySetJobs(JobViewModel, SubJobViewModel);
		}
	}
}

void UHeroJobSystemViewModel::TrySetSubJob(UHeroJobViewModel* JobViewModel)
{
	if (HeroJobSystemComponent && JobViewModel && bSwitchingJobs == false)
	{
		if (MainJobViewModel->GetJobViewModel()->JobDefinition == JobViewModel->GetJobViewModel()->JobDefinition)
		{
			TrySetJobs(SubJobViewModel, JobViewModel);
		}
		else
		{
			TrySetJobs(MainJobViewModel, JobViewModel);
		}
	}
}

void UHeroJobSystemViewModel::SetMainJobViewModel(UHeroJobViewModel* InValue)
{
	if (MainJobViewModel)
	{
		MainJobViewModel->SetIsMainJob(false);
	}
	if (InValue)
	{
		InValue->SetIsMainJob(true);
	}
	UE_MVVM_SET_PROPERTY_VALUE(MainJobViewModel, InValue);
}

void UHeroJobSystemViewModel::SetSubJobViewModel(UHeroJobViewModel* InValue)
{
	if (SubJobViewModel)
	{
		SubJobViewModel->SetIsSubJob(false);
	}
	if (InValue)
	{
		InValue->SetIsSubJob(true);
	}
	UE_MVVM_SET_PROPERTY_VALUE(SubJobViewModel, InValue);
}

UHeroJobViewModel* UHeroJobSystemViewModel::CreateHeroJobViewModel(TSoftObjectPtr<UJobDefinition> JobDefinition)
{
	UHeroJobViewModel* NewVM = NewObject<UHeroJobViewModel>(this);
	UJobViewModel* JobViewModel = NewObject<UJobViewModel>(this);
	NewVM->SetJobViewModel(JobViewModel);
	
	if (UJobDefinition* JobDefPtr = JobDefinition.Get())
	{
		JobViewModel->SetJobDefinition(JobDefPtr);
		if (HeroJobSystemComponent)
		{
			if (HeroJobSystemComponent->GetMainJob() == JobDefPtr)
			{
				SetMainJobViewModel(NewVM);
			}
			if (HeroJobSystemComponent->GetSubJob() == JobDefPtr)
			{
				SetSubJobViewModel(NewVM);
			}
		}
	}
	else
	{
		JobViewModel->LoadJobDefinition(JobDefinition);
	}
	
	if (HeroJobSystemComponent)
	{
		FJobProgressItem JobProgress = HeroJobSystemComponent->FindJobProgressItem(JobDefinition);
		if (JobProgress.IsValid())
		{
			NewVM->SetJobProgress(JobProgress);
		}
	}
	
	HeroJobViewModels.Add(NewVM);
	return NewVM;
}

void UHeroJobSystemViewModel::OnMainJobChanged(UJobDefinition* JobDefinition)
{
	if (JobDefinition)
	{
		bool bFoundExistingJob = false;
	
		for (const TObjectPtr<UHeroJobViewModel>& JobViewModel : HeroJobViewModels)
		{
			if (JobViewModel->GetJobViewModel()->JobDefinition == JobDefinition)
			{
				SetMainJobViewModel(JobViewModel);
				bFoundExistingJob = true;
				break;
			}
		}

		if (!bFoundExistingJob)
		{
			CreateHeroJobViewModel(JobDefinition);
		}
	}
	else
	{
		SetMainJobViewModel(nullptr);
	}
}

void UHeroJobSystemViewModel::OnSubJobChanged(UJobDefinition* JobDefinition)
{
	if (JobDefinition)
	{
		bool bFoundExistingJob = false;
		
		for (const TObjectPtr<UHeroJobViewModel>& JobViewModel : HeroJobViewModels)
		{
			if (JobViewModel->GetJobViewModel()->JobDefinition == JobDefinition)
			{
				SetSubJobViewModel(JobViewModel);
				bFoundExistingJob = true;
				break;
			}
		}
		
		if (!bFoundExistingJob)
		{
			CreateHeroJobViewModel(JobDefinition);
		}
	}
	else
	{
		SetSubJobViewModel(nullptr);
	}
}

void UHeroJobSystemViewModel::OnTrySetJob(bool bSuccess)
{
	UE_MVVM_SET_PROPERTY_VALUE(bSwitchingJobs, false);
}

void UHeroJobSystemViewModel::OnJobProgressUpdated(const FJobProgressItem& JobProgressItem)
{
	for (const TObjectPtr<UHeroJobViewModel>& JobViewModel : HeroJobViewModels)
	{
		if (JobViewModel->GetJobViewModel()->JobDefinition == JobProgressItem.Job.Get())
		{
			JobViewModel->SetJobProgress(JobProgressItem);
			return;
		}
	}
}

void UHeroJobSystemViewModel::OnHeroJobSystemDataUpdated(const FHeroJobSystemData& Data)
{
	HeroJobSystemData = Data;
	
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetLevel);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetExperience);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetMaxLevel);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetMaxJobLevel);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(IsSubJobUnlocked);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetPercentageTowardsNextLevel);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetExperienceRequiredUntilNextLevel);
}

void UHeroJobSystemViewModel::InitializeStartingData()
{
	OnHeroJobSystemDataUpdated(HeroJobSystemComponent->GetHeroJobSystemData());
	
	UJobDefinition* MainJob = HeroJobSystemComponent->GetMainJob();
	UJobDefinition* SubJob = HeroJobSystemComponent->GetSubJob();
	bool bFoundMainJob = false;
	bool bFoundSubJob = false;
	for (TObjectPtr<UHeroJobViewModel>& ViewModel : HeroJobViewModels)
	{
		UJobDefinition* JobDef = ViewModel->GetJobViewModel()->JobDefinition;
		FJobProgressItem JobProgress = HeroJobSystemComponent->FindJobProgressItem(JobDef);
		ViewModel->SetJobProgress(JobProgress);
		if (!JobProgress.IsValid())
		{
			ViewModel->SetIsJobAvailable(false);
		}
		if (JobDef == MainJob)
		{
			SetMainJobViewModel(ViewModel);
			bFoundMainJob = true;
		}
		if (JobDef == SubJob)
		{
			SetSubJobViewModel(ViewModel);
			bFoundSubJob = true;
		}
	}
		
	if (!bFoundMainJob)
	{
		if (MainJob)
		{
			CreateHeroJobViewModel(MainJob);
		}
		else
		{
			SetMainJobViewModel(nullptr);
		}
	}
	if (!bFoundSubJob)
	{
		if (SubJob)
		{
			CreateHeroJobViewModel(SubJob);
		}
		else
		{
			SetSubJobViewModel(nullptr);
		}
	}
}

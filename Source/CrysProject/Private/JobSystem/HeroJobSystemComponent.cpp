// Copyright Soccertitan 2026


#include "JobSystem/HeroJobSystemComponent.h"

#include "AbilitySystem/AttributeSet/JobAttributeSet.h"
#include "AbilitySystem/AttributeSet/PrimaryAttributeSet.h"
#include "JobSystem/JobDefinition.h"
#include "JobSystem/JobSystemBlueprintFunctionLibrary.h"
#include "Net/UnrealNetwork.h"
#include "System/CrysAssetManager.h"


UHeroJobSystemComponent::UHeroJobSystemComponent()
{
	SetIsReplicatedByDefault(true);
}

void UHeroJobSystemComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ThisClass, JobProgressContainer);
	
	FDoRepLifetimeParams Params;
	Params.bIsPushBased = true;
	Params.Condition = COND_OwnerOnly;
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, HeroJobSystemData, Params);
}

void UHeroJobSystemComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (JobProgressContainer.Items.IsEmpty())
	{
		RestoreJobProgress(StartingJobProgress);
	}
}

void UHeroJobSystemComponent::TrySetJobs(UJobDefinition* InMainJob, UJobDefinition* InSubJob)
{
	if (!HasAuthority())
	{
		Server_TrySetJobs(InMainJob, InSubJob);
		return;
	}
	
	if (!InMainJob || InMainJob == InSubJob || !GetAbilitySystemComponent())
	{
		Client_OnTrySetJobs(false);
		return;
	}
	
	FJobParams JobParams = MakeJobParams();
	const FJobProgressItem MainJobProgress = FindJobProgressItem(InMainJob);
	if (!MainJobProgress.IsValid())
	{
		Client_OnTrySetJobs(false);
		return;
	}
	
	JobParams.MainJob = InMainJob;
	JobParams.SubJobEffectiveness = HeroJobSystemData.SubJobEffectiveness;
	JobParams.bMaximizeHpMp = true;
	
	if (HeroJobSystemData.bSubJobUnlocked)
	{
		const FJobProgressItem SubJobProgress = FindJobProgressItem(InSubJob);
		if (!SubJobProgress.IsValid())
		{
			JobParams.SubJob = nullptr;
			JobParams.SubJobLevel = 0;
		}
		else
		{
			JobParams.SubJob = InSubJob;
			JobParams.SubJobLevel = SubJobProgress.Level;
		}
	}
	else
	{
		JobParams.SubJob = nullptr;
		JobParams.SubJobLevel = 0;
	}
	
	SetJobs(JobParams);
	Client_OnTrySetJobs(true);
}

const FHeroJobSystemData& UHeroJobSystemComponent::GetHeroJobSystemData() const
{
	return HeroJobSystemData;
}

void UHeroJobSystemComponent::SetHeroJobSystemData(const FHeroJobSystemData& InData)
{
	if (!HasAuthority())
	{
		return;
	}
	
	const bool bLevelChanged = HeroJobSystemData.Level != InData.Level;
	const bool bSubJobEffectivenessChanged = HeroJobSystemData.SubJobEffectiveness != InData.SubJobEffectiveness;
	
	HeroJobSystemData = InData;
	OnHeroJobSystemDataUpdatedDelegate.Broadcast(HeroJobSystemData);
	MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, HeroJobSystemData, this);
	
	if (bLevelChanged)
	{
		OverrideBaseAttribute(HeroJobSystemData.Level, UPrimaryAttributeSet::GetLevelAttribute());
	}
	if (bSubJobEffectivenessChanged)
	{
		OverrideBaseAttribute(HeroJobSystemData.SubJobEffectiveness, UJobAttributeSet::GetSubJobEffectivenessAttribute());
	}
	
	if (GetSubJob() && HeroJobSystemData.bSubJobUnlocked == false)
	{
		FJobParams Params = MakeJobParams();
		Params.SubJob = nullptr;
		Params.SubJobLevel = 0;
		Params.bMaximizeHpMp = true;
		SetJobs(Params);
	}
}

void UHeroJobSystemComponent::Multi_OnLevelUp_Implementation(const int32 OldLevel)
{
	OnLevelUpDelegate.Broadcast(OldLevel);
}

void UHeroJobSystemComponent::Multi_OnLevelDown_Implementation(const int32 OldLevel)
{
	OnLevelDownDelegate.Broadcast(OldLevel);
}

TArray<FJobProgressItem> UHeroJobSystemComponent::GetJobProgressItems() const
{
	return JobProgressContainer.Items;
}

FJobProgressItem UHeroJobSystemComponent::FindJobProgressItem(const TSoftObjectPtr<UJobDefinition>& Job) const
{
	if (FJobProgressItem* Item = JobProgressContainer.FindJobProgressItem(Job))
	{
		return *Item;
	}
	return FJobProgressItem();
}

void UHeroJobSystemComponent::AddOrSetJobProgressItem(const FJobProgressItem& InJobProgressItem)
{
	if (!HasAuthority())
	{
		return;
	}

	if (!InJobProgressItem.IsValid())
	{
		return;
	}

	if (FJobProgressItem* ProgressItem = JobProgressContainer.FindJobProgressItem(InJobProgressItem.Job))
	{
		ProgressItem->Level = InJobProgressItem.Level;
		ProgressItem->Experience = InJobProgressItem.Experience;
		JobProgressContainer.MarkItemDirty(*ProgressItem);
		OnJobProgressUpdatedDelegate.Broadcast(*ProgressItem);
	}
	else
	{
		JobProgressContainer.AddJobProgressItem(InJobProgressItem);
	}

	if (GetMainJob() == InJobProgressItem.Job)
	{
		OverrideBaseAttribute(InJobProgressItem.Level, UJobAttributeSet::GetMainJobLevelAttribute());
	}
	
	if (GetSubJob() == InJobProgressItem.Job)
	{
		OverrideBaseAttribute(InJobProgressItem.Level, UJobAttributeSet::GetSubJobLevelAttribute());
	}
}

void UHeroJobSystemComponent::AddExperienceForJob(const UJobDefinition* Job, const int32 Experience)
{
	if (!HasAuthority() || Experience <= 0)
	{
		return;
	}

	JobProgressContainer.AddExperience(Job, Job->ExperienceRequirement, Experience);
}

void UHeroJobSystemComponent::RestoreJobProgress(const TArray<FJobProgressItem>& JobProgressItems)
{
	if (!HasAuthority())
	{
		return;
	}
	
	FJobParams JobParams = MakeJobParams();
	JobParams.RaceLevel = HeroJobSystemData.Level;
	JobParams.SubJobEffectiveness = HeroJobSystemData.SubJobEffectiveness;
	
	JobProgressContainer.Reset();
	bool bHaveMainJob = false;
	bool bHaveSubJob = false;
	
	for (const FJobProgressItem& Item : JobProgressItems)
	{
		if (Item.IsValid())
		{
			if (bHaveMainJob == false && Item.Job == GetMainJob())
			{
				bHaveMainJob = true;
				JobParams.MainJobLevel = Item.Level;
			}
			
			if (bHaveSubJob == false && Item.Job == GetSubJob())
			{
				bHaveSubJob = true;
				JobParams.SubJobLevel = Item.Level;
			}
			JobProgressContainer.AddJobProgressItem(Item);
		}
	}
	
	if (bHaveMainJob == false)
	{
		JobParams.MainJob = nullptr;
		JobParams.MainJobLevel = 0;
		
		if (JobProgressItems.IsValidIndex(0))
		{
			JobParams.MainJob = UCrysAssetManager::Get().GetAsset(JobProgressItems[0].Job, false);
			JobParams.MainJobLevel = JobProgressItems[0].Level;
		}
	}
	if (HeroJobSystemData.bSubJobUnlocked == false || bHaveSubJob == false)
	{
		JobParams.SubJob = nullptr;
		JobParams.SubJobLevel = 0;
	}
	
	JobParams.bMaximizeHpMp = true;
	SetJobs(JobParams);
}

bool UHeroJobSystemComponent::AddExperience(const int32 Experience)
{
	if (!HasAuthority() || Experience == 0 || !GetRace())
	{
		return false;
	}

	bool bLevelUpdated = false;
	int32 OldLevel = HeroJobSystemData.Level;

	if (Experience > 0)
	{
		bLevelUpdated = UJobSystemBlueprintFunctionLibrary::AddExperience(HeroJobSystemData.Level, HeroJobSystemData.Experience, 
			Experience, GetRace()->ExperienceRequirement, HeroJobSystemData.MaxLevel);
	}
	else
	{
		bLevelUpdated = UJobSystemBlueprintFunctionLibrary::SubtractExperience(HeroJobSystemData.Level, HeroJobSystemData.Experience, 
			FMath::Abs(Experience), GetRace()->ExperienceRequirement);
	}
	
	if (bLevelUpdated)
	{
		if (HeroJobSystemData.Level > OldLevel)
		{
			Multi_OnLevelUp(OldLevel);
		}
		else
		{
			Multi_OnLevelDown(OldLevel);
		}
		OverrideBaseAttribute(HeroJobSystemData.Level, UPrimaryAttributeSet::GetLevelAttribute());
	}

	OnHeroJobSystemDataUpdatedDelegate.Broadcast(HeroJobSystemData);
	MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, HeroJobSystemData, this);
	return bLevelUpdated;
}

void UHeroJobSystemComponent::OnRegister()
{
	Super::OnRegister();
	
	JobProgressContainer.Owner = this;
}

void UHeroJobSystemComponent::OnRep_HeroJobSystemData()
{
	OnHeroJobSystemDataUpdatedDelegate.Broadcast(HeroJobSystemData);
}

void UHeroJobSystemComponent::OnJobLevelUpInternal(const FJobProgressItem& JobProgressItem, const int32 OldLevel)
{
	if (JobProgressItem.Job == GetMainJob())
	{
		OverrideBaseAttribute(JobProgressItem.Level, UJobAttributeSet::GetMainJobLevelAttribute());
	}
	
	if (JobProgressItem.Job == GetSubJob())
	{
		OverrideBaseAttribute(JobProgressItem.Level, UJobAttributeSet::GetSubJobLevelAttribute());
	}
	
	Multi_OnJobLevelUp(JobProgressItem, OldLevel);
}

void UHeroJobSystemComponent::Multi_OnJobLevelUp_Implementation(const FJobProgressItem& JobProgressItem, const int32 OldLevel)
{
	OnJobLevelUpDelegate.Broadcast(JobProgressItem, OldLevel);
}

void UHeroJobSystemComponent::Server_TrySetJobs_Implementation(const UJobDefinition* InMainJob, const UJobDefinition* InSubJob)
{
	TrySetJobs(const_cast<UJobDefinition*>(InMainJob), const_cast<UJobDefinition*>(InSubJob));
}

void UHeroJobSystemComponent::Client_OnTrySetJobs_Implementation(bool bSuccess)
{
	OnTrySetJobDelegate.Broadcast(bSuccess);
}



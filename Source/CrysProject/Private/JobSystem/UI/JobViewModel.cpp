// Copyright Soccertitan 2026


#include "JobSystem/UI/JobViewModel.h"

#include "Engine/AssetManager.h"
#include "JobSystem/JobDefinition.h"


bool UJobViewModel::IsJobViewModelValid() const
{
	return JobDefinition ? true : false;
}

FText UJobViewModel::GetJobName() const
{
	return JobDefinition ? JobDefinition->JobName : FText();
}

FText UJobViewModel::GetShortName() const
{
	return JobDefinition ? JobDefinition->ShortName : FText();
}

FText UJobViewModel::GetDescription() const
{
	return JobDefinition ? JobDefinition->Description : FText();
}

void UJobViewModel::SetJobDefinition(UJobDefinition* InHeroJob)
{
	JobDefStreamableHandle.Reset();
	JobDefinition = InHeroJob;
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(IsJobViewModelValid);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetJobName);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetShortName);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetDescription);
}

void UJobViewModel::LoadJobDefinition(TSoftObjectPtr<UJobDefinition> InJobDefinition)
{
	if (!InJobDefinition.IsNull())
	{
		FStreamableDelegate Delegate = FStreamableDelegate::CreateUObject(this, &UJobViewModel::OnJobDefinitionLoaded, InJobDefinition);
		JobDefStreamableHandle = UAssetManager::Get().PreloadPrimaryAssets({InJobDefinition->GetPrimaryAssetId()}, {}, false, Delegate);
	}
}

void UJobViewModel::OnJobDefinitionLoaded(TSoftObjectPtr<UJobDefinition> InJobDefinition)
{
	SetJobDefinition(InJobDefinition.Get());
}

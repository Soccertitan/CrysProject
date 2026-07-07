// Copyright Soccertitan 2026


#include "JobSystem/JobContainer.h"

#include "JobSystem/JobDefinition.h"
#include "Settings/CrysGameData.h"
#include "System/CrysAssetManager.h"
#include "UObject/ObjectSaveContext.h"

#define LOCTEXT_NAMESPACE "JobContainer"

FTextFormat UJobContainer::TextFormat = LOCTEXT("GeneratedJobText", "{Start}/{Append}");

#if WITH_EDITORONLY_DATA
void UJobContainer::PreSave(FObjectPreSaveContext SaveContext)
{
	Super::PreSave(SaveContext);
	
	UpdateJobText();
}

void UJobContainer::UpdateJobText()
{
	JobText = FText();

	for (const TSoftObjectPtr<UJobDefinition> Job : Jobs)
	{
		const UJobDefinition* LoadedJob = UCrysAssetManager::GetAsset(Job, false);
		if (LoadedJob)
		{
			FText JobShortName = LoadedJob->ShortName;
			if (JobText.IsEmpty())
			{
				JobText = JobShortName;
			}
			else
			{
				FFormatNamedArguments Args;
				Args.Add("Start", JobText);
				Args.Add("Append", JobShortName);
				JobText = FText::Format(TextFormat, Args);
			}
		}
	}
}
#endif

#undef LOCTEXT_NAMESPACE

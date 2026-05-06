// Copyright Soccertitan 2026


#include "JobSystem/JobDefinition.h"

#include "UObject/AssetRegistryTagsContext.h"

FPrimaryAssetId UJobDefinition::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(StaticClass()->GetFName(), GetFName());
}

void UJobDefinition::GetAssetRegistryTags(FAssetRegistryTagsContext Context) const
{
	Super::GetAssetRegistryTags(Context);

	// Adding the JobTag to the AssetRegistry
	FAssetRegistryTag RegistryTag;
	RegistryTag.Type = FAssetRegistryTag::TT_Alphabetical;
	RegistryTag.Name = "JobTag";
	RegistryTag.Value = JobTag.ToString();
	Context.AddTag(RegistryTag);
}
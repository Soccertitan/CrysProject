// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "JobTypes.h"
#include "ScalableFloat.h"
#include "Engine/DataAsset.h"
#include "JobDefinition.generated.h"

class UGameplayEffect;

USTRUCT()
struct FJobDefinitionGameplayEffects
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, meta = (ClampMin = 0))
	int32 Level = 0;
	
	UPROPERTY(EditAnywhere)
	TArray<TSoftClassPtr<UGameplayEffect>> GameplayEffects;
};

/**
 * Information about a Job that characters can be.
 */
UCLASS(Const)
class CRYSPROJECT_API UJobDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
	virtual void GetAssetRegistryTags(FAssetRegistryTagsContext Context) const override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Job")
	FText JobName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Job")
	FText ShortName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Job", meta = (MultiLine))
	FText Description;

	/** The tag to add to the AbilitySystemComponent when this job is equipped. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Job", meta = (Categories = "Job"))
	FGameplayTag JobTag;

	/** The amount of experience required to level up. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Job")
	FScalableFloat ExperienceRequirement;
	
	/** Base attributes to apply. */
	UPROPERTY(EditDefaultsOnly, Category = "Job")
	FPrimaryAttributes BaseAttributes;

	/** GameplayEffects to apply if the Channel0 (Base) level is greater than or equal to the effect. The effects are granted at the level. */
	UPROPERTY(EditDefaultsOnly, Category = "Job", meta = (AssetBundles = "Ability", TitleProperty = "Level: {Level}"))
	TArray<FJobDefinitionGameplayEffects> GameplayEffectsForLevels;
};

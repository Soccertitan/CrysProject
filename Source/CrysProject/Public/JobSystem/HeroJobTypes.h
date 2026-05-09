// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "HeroJobTypes.generated.h"

struct FScalableFloat;
class UJobDefinition;

/** Used in the HeroJobSystemComponent to store data related to Jobs and overall character level. */
USTRUCT(BlueprintType)
struct FHeroJobSystemData
{
	GENERATED_BODY()
	
	/** The overall level of the character. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 1))
	int32 Level = 1;

	/** The total cumulative amount of experience achieved by the character. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 0))
	int32 Experience = 0;
	
	/** The MaxLevel a character can obtain from leveling. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 1))
	int32 MaxLevel = 50;
	
	/** The max level any job can achieve. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 1))
	int32 MaxJobLevel = 20;
	
	/** Allows the character to equip a SubJob. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bSubJobUnlocked = false;
	
	/** A multiplier applied that affects the SubJobs attribute multiplier. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 0))
	float SubJobEffectiveness = 0.2f;
};

/** Contains data about the progress of a job. */
USTRUCT(BlueprintType)
struct FJobProgressItem : public FFastArraySerializerItem
{
	GENERATED_BODY()

	// The identifier for this progress item.
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UJobDefinition> Job;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = 1))
	int32 Level = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = 0))
	int32 Experience = 0;

	//~ Begin of FFastArraySerializerItem
	void PostReplicatedAdd(const struct FJobProgressContainer& InSerializer);
	void PostReplicatedChange(const FJobProgressContainer& InSerializer);
	//~ End of FFastArraySerializerItem

	bool IsValid() const;

private:
	friend struct FJobProgressContainer;
	friend class UHeroJobSystemComponent;
};

USTRUCT(BlueprintType)
struct FJobProgressContainer : public FFastArraySerializer
{
	GENERATED_BODY()

	FJobProgressContainer() {}

	void AddJobProgressItem(const FJobProgressItem& NewItem);

	/**
	 * Adds the specified amount of experience to the item with the JobTag. If it goes beyond the required exp for
	 * the level, it will add levels to the item.
	 */
	void AddExperience(const UJobDefinition* Job, const FScalableFloat& ExperienceRequirement, int32 Experience);

	FJobProgressItem* FindJobProgressItem(const TSoftObjectPtr<UJobDefinition>& Job) const;

	/** Removes all progress in the Container. */
	void Reset();

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FastArrayDeltaSerialize<FJobProgressItem, FJobProgressContainer>(Items, DeltaParms, *this);
	}
	
private:
	UPROPERTY(EditAnywhere)
	TArray<FJobProgressItem> Items;

	UPROPERTY(NotReplicated)
	TObjectPtr<UHeroJobSystemComponent> Owner;

	friend struct FJobProgressItem;
	friend UHeroJobSystemComponent;
	
	void AddExperienceInternal(FJobProgressItem& Item, const FScalableFloat& ExperienceRequirement, int32 Experience);
};
template<>
struct TStructOpsTypeTraits<FJobProgressContainer> : TStructOpsTypeTraitsBase2<FJobProgressContainer>
{
	enum { WithNetDeltaSerializer = true };
};

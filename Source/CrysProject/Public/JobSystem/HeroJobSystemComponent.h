// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "HeroJobTypes.h"
#include "JobSystemComponent.h"
#include "HeroJobSystemComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHeroJobSystemDataSignature, const FHeroJobSystemData&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHeroJobSystemLevelUpdatedSignature, int32, OldLevel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHeroJobSystemJobProgressUpdatedSignature, const FJobProgressItem&, JobProgressItem);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHeroJobSystemJobLevelUpSignature, const FJobProgressItem&, JobProgressItem, int32, OldLevel);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CRYSPROJECT_API UHeroJobSystemComponent : public UJobSystemComponent
{
	GENERATED_BODY()
	
	friend struct FJobProgressContainer;

public:
	UHeroJobSystemComponent();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable, Category = "JobSystem")
	void TrySetJobs(UJobDefinition* InMainJob, UJobDefinition* InSubJob);
	
	UFUNCTION(BlueprintPure, Category = "JobSystem")
	const FHeroJobSystemData& GetHeroJobSystemData() const;
	
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "JobSystem")
	void SetHeroJobSystemData(const FHeroJobSystemData& InData);

	/** [Client + Server] Whenever the JobManagerData is updated. */
	UPROPERTY(BlueprintAssignable, DisplayName = "OnHeroJobSystemDataUpdated")
	FHeroJobSystemDataSignature OnHeroJobSystemDataUpdatedDelegate;
	/** [Server + UnreliableMulticast] When the Character levels up. */
	UPROPERTY(BlueprintAssignable, DisplayName = "OnLevelUp")
	FHeroJobSystemLevelUpdatedSignature OnLevelUpDelegate;
	/** [Server + UnreliableMulticast] When the Character levels down. */
	UPROPERTY(BlueprintAssignable, DisplayName = "OnLevelDown")
	FHeroJobSystemLevelUpdatedSignature OnLevelDownDelegate;
	
	/** [Server + UnreliableMulticast] Called when a Job levels up. */
	UPROPERTY(BlueprintAssignable, DisplayName = "OnJobLevelUp")
	FHeroJobSystemJobLevelUpSignature OnJobLevelUpDelegate;
	/** [Client + Server] whenever a JobProgressItem is updated. */
	UPROPERTY(BlueprintAssignable, DisplayName = "OnJobProgressUpdated")
	FHeroJobSystemJobProgressUpdatedSignature OnJobProgressUpdatedDelegate;
	
	/** [Owning Client] Called when setting the Job succeeds or fails. */
	UPROPERTY(BlueprintAssignable, DisplayName = "OnTrySetJob")
	FJobSystemComponentBoolSignature OnTrySetJobDelegate;
	
	/** @return A copy of all the JobProgressItems. */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "JobSystem")
	TArray<FJobProgressItem> GetJobProgressItems() const;
	
	/** Returns a copy of the JobProgressItem. */
	UFUNCTION(BlueprintPure, Category = "JobSystem")
	FJobProgressItem FindJobProgressItem(const TSoftObjectPtr<UJobDefinition>& Job) const;
	
	/**
	 * Adds a new JobProgressItem or updates an existing one. Then, if the Job matches the currently equipped job
	 * update the attributes.
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "JobSystem")
	void AddOrSetJobProgressItem(const FJobProgressItem& InJobProgressItem);
	
	/**
	 * Adds the specified amount of experience for the Job. If it goes beyond the required exp for level it will
	 * add levels to the item.
	 * @param Job The job to add experience to.
	 * @param Experience The amount of experience to add.
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "JobSystem")
	void AddExperienceForJob(const UJobDefinition* Job, const int32 Experience);
	
	/**
	 * Resets the current progress, restores the progress to the JobProgressContainer, and sets the jobs.
	 * @param InJobProgressItems The progress to restore.
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "JobSystem")
	void RestoreJobProgress(const TArray<FJobProgressItem>& InJobProgressItems);
	
	/**
	 * Adds the specified amount of experience for the character to the HeroJobSystemData. If it goes beyond the required exp for level
	 * it will add levels and call the LevelUp or LevelDown event.
	 * @param Experience The amount of experience to add.
	 * @return True, if the level changed.
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "JobSystem")
	bool AddExperience(const int32 Experience);
	
protected:
	virtual void OnRegister() override;
	
	UFUNCTION()
	virtual void OnRep_HeroJobSystemData();
	
private:
	/** The overall progress of the Hero. */
	UPROPERTY(EditAnywhere, ReplicatedUsing=OnRep_HeroJobSystemData, Category = "JobSystem")
	FHeroJobSystemData HeroJobSystemData;
	
	/** Contains the replicated container of character's Job progress. */
	UPROPERTY(Replicated)
	FJobProgressContainer JobProgressContainer;
	
	/** Initializes the JobProgressContainer with these values. */
	UPROPERTY(EditAnywhere, Category = "JobSystem")
	TArray<FJobProgressItem> StartingJobProgress;
	
	UFUNCTION(NetMulticast, Unreliable)
	void Multi_OnLevelUp(const int32 OldLevel);
	UFUNCTION(NetMulticast, Unreliable)
	void Multi_OnLevelDown(const int32 OldLevel);
	
	/** Calls Multicast event Multi_OnLevelUp and updates the current level of the Job if applicable. */
	void OnJobLevelUpInternal(const FJobProgressItem& JobProgressItem, const int32 OldLevel);

	/** Broadcasts the OnLevelUp delegate. */
	UFUNCTION(NetMulticast, Unreliable)
	void Multi_OnJobLevelUp(const FJobProgressItem& JobProgressItem, const int32 OldLevel);
	
	UFUNCTION(Server, Reliable)
	void Server_TrySetJobs(const UJobDefinition* InMainJob, const UJobDefinition* InSubJob);
	
	UFUNCTION(Client, Reliable)
	void Client_OnTrySetJobs(bool bSuccess);
};

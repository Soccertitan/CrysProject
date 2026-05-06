// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "CrimAbilitySystemInterface.h"
#include "Components/ActorComponent.h"
#include "JobSystemComponent.generated.h"


struct FStreamableHandle;
struct FGameplayAttribute;
struct FActiveGameplayEffectHandle;
struct FOnAttributeChangeData;
class UCrimAbilitySystemComponent;
class UJobDefinition;

/** The JobSystem returns this info when querying GetJobs. */
USTRUCT(BlueprintType)
struct FJobInfo
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UJobDefinition> Race;
	UPROPERTY(BlueprintReadOnly)
	int32 RaceLevel = 0;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UJobDefinition> MainJob;
	UPROPERTY(BlueprintReadOnly)
	int32 MainJobLevel = 0;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UJobDefinition> SubJob;
	UPROPERTY(BlueprintReadOnly)
	int32 SubJobLevel = 0;
	UPROPERTY(BlueprintReadOnly)
	float SubJobEffectiveness = 0.f;
};

/** Used by JobSystem in SetJobs. */
USTRUCT(BlueprintType)
struct FJobParams
{
	GENERATED_BODY()
	
	FJobParams(){}
	FJobParams(const FJobInfo& Info);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UJobDefinition> Race;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 RaceLevel = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UJobDefinition> MainJob;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MainJobLevel = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UJobDefinition> SubJob;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SubJobLevel = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SubJobEffectiveness = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bMaximizeHpMp = false;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FJobSystemComponentBoolSignature, bool, bValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FJobSystemComponentJobSignature, UJobDefinition*, Definition);

/**
 * Applies GameplayEffects and set Attributes based on the Jobs, Race, and levels.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CRYSPROJECT_API UJobSystemComponent : public UActorComponent, public ICrimAbilitySystemInterface
{
	GENERATED_BODY()

public:
	UJobSystemComponent();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;
	virtual void PreNetReceive() override;
	
	/** Sets the Race and Jobs to the specified values and grants GameplayEffects according to that level. */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "JobSystem")
	void SetJobs(FJobParams JobParams);
	
	/** Returns the base values from the ASC for the levels. Also returns the current Race and Jobs. */
	UFUNCTION(BlueprintPure, Category = "JobSystem")
	FJobInfo GetJobs() const;
	
	/** Can only return true on the server when changing jobs. */
	UFUNCTION(BlueprintPure, Category = "ChangingJobs")
	bool IsChangingJobs() const { return bChangingJobs; }
	
	/** [Server] Called at the start of SetJobs and when the function finishes. */
	UPROPERTY(BlueprintAssignable, DisplayName = "OnChangingJobs")
	FJobSystemComponentBoolSignature OnChangingJobsDelegate;
	
	/** [Client + Server] When the Race changes. */
	UPROPERTY(BlueprintAssignable, DisplayName = "OnRaceChanged")
	FJobSystemComponentJobSignature OnRaceChangedDelegate;
	/** [Client + Server] When the MainJob changes. */
	UPROPERTY(BlueprintAssignable, DisplayName = "OnMainJobChanged")
	FJobSystemComponentJobSignature OnMainJobChangedDelegate;
	/** [Client + Server] When the SubJob changes. */
	UPROPERTY(BlueprintAssignable, DisplayName = "OnSubJobChanged")
	FJobSystemComponentJobSignature OnSubJobChangedDelegate;
	
	// ICrimAbilitySystemInterface
	virtual void SetCrimAbilitySystem_Implementation(UCrimAbilitySystemComponent* AbilitySystemComponent) override;
	
	UFUNCTION(BlueprintPure, Category = "JobSystem")
	bool HasAuthority() const;
	
protected:
	/** Called in SetCrimAbilitySystem. */
	virtual void RemoveBindingToAttributeDelegates();
	/** Called in SetCrimAbilitySystem. */
	virtual void BindToAttributeDelegates();
	
	virtual void OnAttributeChanged(const FOnAttributeChangeData& Data);
	
	void OverrideBaseAttribute(const float Value, const FGameplayAttribute& Attribute);
	
	UFUNCTION()
	virtual void OnRep_Race();
	UFUNCTION()
	virtual void OnRep_MainJob();
	UFUNCTION()
	virtual void OnRep_SubJob();
	
private:
	/** The current Race the character is. */
	UPROPERTY(EditAnywhere, ReplicatedUsing=OnRep_Race, Category = "JobManager")
	TObjectPtr<UJobDefinition> Race;
	
	/** The currently chosen MainJob. */
	UPROPERTY(EditAnywhere, ReplicatedUsing=OnRep_MainJob, Category = "JobManager")
	TObjectPtr<UJobDefinition> MainJob;
	
	/** The current chosen SubJob. */
	UPROPERTY(EditAnywhere, ReplicatedUsing=OnRep_SubJob, Category = "JobManager")
	TObjectPtr<UJobDefinition> SubJob;
	
	/** Cached ASC from the owner. */
	UPROPERTY()
	TObjectPtr<UCrimAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> RaceActiveGameplayEffectHandles;
	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> MainJobActiveGameplayEffectHandles;
	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> SubJobActiveGameplayEffectHandles;
	
	/** If true, the BaseAttribute will not be updated. This will be true when changing jobs and levels. */
	bool bChangingJobs = false;

	/**
	 * Called when anything attributes related to the job or race changes. Or if a different race/job is set.
	 * Overrides the base attributes on the ASC with the base attribute values from the equipped Jobs and Race.
	 */
	void ApplyBaseAttributes() const;
	
	/** Sets the HP and MP attributes to the maximum value. */
	void MaximizeHpMpAttributes();
	
	/** Cached value of whether our owner is a simulated Actor. */
	bool bCachedIsNetSimulated = false;
	void CacheIsNetSimulated();
	
	/** Grants GameplayEffects for the job at the specified level. */
	void GrantGameplayEffects(const UJobDefinition* Job, const int32 Level, TArray<FActiveGameplayEffectHandle>& OutHandles);
	
	void RemoveActiveGameplayEffects(TArray<FActiveGameplayEffectHandle>& InHandles);
};

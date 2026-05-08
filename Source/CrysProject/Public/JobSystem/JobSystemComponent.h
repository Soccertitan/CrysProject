// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "CrimAbilitySystemInterface.h"
#include "JobTypes.h"
#include "Components/ActorComponent.h"
#include "JobSystemComponent.generated.h"


struct FJobParams;
struct FStreamableHandle;
struct FGameplayAttribute;
struct FOnAttributeChangeData;
class UCrimAbilitySystemComponent;
class UJobDefinition;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FJobSystemComponentBoolSignature, bool, bValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FJobSystemComponentJobSignature, UJobDefinition*, Definition);

/**
 * Applies GameplayEffects and set Attributes based on the Jobs, Race, and levels. The Primary attributes are set to 
 * a base level anytime Level,MainJob,SubJob,SubJobEffectiveness value changes. The GameplayEffects are updated depending
 * on if the "base" channel0 is different.
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
	
	/** Fills out the JobParms with current jobs and base attribute value from the AbilitySystemComponent. */
	UFUNCTION(BlueprintPure, Category = "JobSystem")
	FJobParams MakeJobParams() const;
	
	UFUNCTION(BlueprintPure, Category = "JobSystem")
	UJobDefinition* GetRace() const { return Race; }
	UFUNCTION(BlueprintPure, Category = "JobSystem")
	UJobDefinition* GetMainJob() const { return MainJob; }
	UFUNCTION(BlueprintPure, Category = "JobSystem")
	UJobDefinition* GetSubJob() const { return SubJob; }
	
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
	UCrimAbilitySystemComponent* GetAbilitySystemComponent() const;
	
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
	UPROPERTY(EditAnywhere, ReplicatedUsing=OnRep_Race, Category = "JobSystem")
	TObjectPtr<UJobDefinition> Race;
	
	/** The currently chosen MainJob. */
	UPROPERTY(EditAnywhere, ReplicatedUsing=OnRep_MainJob, Category = "JobSystem")
	TObjectPtr<UJobDefinition> MainJob;
	
	/** The current chosen SubJob. */
	UPROPERTY(EditAnywhere, ReplicatedUsing=OnRep_SubJob, Category = "JobSystem")
	TObjectPtr<UJobDefinition> SubJob;
	
	/** Cached ASC from the owner. */
	UPROPERTY()
	TObjectPtr<UCrimAbilitySystemComponent> AbilitySystemComponent;
	
	FJobSystemActiveGameplayEffects ActiveGameplayEffects;
	
	/** If true, the BaseAttribute will not be updated. This will be true when changing jobs and levels. */
	bool bChangingJobs = false;

	/**
	 * Called when anything attributes related to the job or race changes. Or if a different race/job is set.
	 * Overrides the base attributes on the ASC with the base attribute values from the equipped Jobs and Race.
	 */
	void ApplyBaseAttributes() const;

	/** Cached value of whether our owner is a simulated Actor. */
	bool bCachedIsNetSimulated = false;
	void CacheIsNetSimulated();

	/**
	 * Applies gameplay effects for the Race and Jobs.
	 */
	void ApplyGameplayEffects();
	
	/** Sets the HP and MP attributes to the maximum value. */
	void MaximizeHpMpAttributes();
	
	/** 
	 * Grants/Updates active gameplay effects that meet the level requirement. If it does not meet the level requirement, 
	 * removes the gameplay effects granted for those levels.
	 */
	void UpdateActiveGameplayEffects(const UJobDefinition* Job, const FGameplayAttribute& Attribute, FJobSystemActiveGameplayEffectHandles& InHandles);
	
	/** Removes all ActiveGameplayEffects */
	void RemoveActiveGameplayEffects(FJobSystemActiveGameplayEffectHandles& InHandles);
};

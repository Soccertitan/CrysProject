// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "Engine/StreamableManager.h"
#include "JobSystem/HeroJobTypes.h"
#include "JobViewModel.generated.h"

/**
 * Information on the Job and progress. Managed by the HeroJobManager.
 */
UCLASS()
class CRYSPROJECT_API UJobViewModel : public UMVVMViewModelBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, FieldNotify, Category = "Viewmodel|Job")
	bool IsJobViewModelValid() const;
	
	UFUNCTION(BlueprintPure, FieldNotify, Category = "Viewmodel|Job")
	FText GetJobName() const;

	UFUNCTION(BlueprintPure, FieldNotify, Category = "Viewmodel|Job")
	FText GetShortName() const;

	UFUNCTION(BlueprintPure, FieldNotify, Category = "Viewmodel|Job")
	FText GetDescription() const;

	void SetJobDefinition(UJobDefinition* InHeroJob);
	UJobDefinition* GetJobDefinition() const { return JobDefinition; }
	
	void LoadJobDefinition(TSoftObjectPtr<UJobDefinition> JobDefinition);
	
private:
	/** The cached value of the JobDefinition. */
	UPROPERTY()
	TObjectPtr<UJobDefinition> JobDefinition;
	
	/** Cached handle for the JobDefinition. */
	TSharedPtr<FStreamableHandle> JobDefStreamableHandle;
	
	void OnJobDefinitionLoaded(TSoftObjectPtr<UJobDefinition> JobDefinition);
};

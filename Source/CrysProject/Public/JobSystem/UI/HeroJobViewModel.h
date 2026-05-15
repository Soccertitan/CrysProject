// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "JobSystem/HeroJobTypes.h"
#include "HeroJobViewModel.generated.h"

class UJobViewModel;
/**
 * Contains additional information from a JobViewModel.
 */
UCLASS()
class CRYSPROJECT_API UHeroJobViewModel : public UMVVMViewModelBase
{
	GENERATED_BODY()
	
public:
	bool IsJobAvailable() const { return bJobAvailable; }
	bool IsMainJob() const { return bMainJob; }
	bool IsSubJob() const { return bSubJob; }
	UJobViewModel* GetJobViewModel() const { return JobViewModel; }
	
	UFUNCTION(BlueprintPure, FieldNotify, Category = "Viewmodel|Job")
	int32 GetLevel() const { return JobProgressItem.Level; }
	UFUNCTION(BlueprintPure, FieldNotify, Category = "Viewmodel|Job")
	int32 GetExperience() const { return JobProgressItem.Experience; }
	UFUNCTION(BlueprintPure, FieldNotify, Category = "Viewmodel|Job")
	int32 GetExperienceRequiredUntilNextLevel() const;
	UFUNCTION(BlueprintPure, FieldNotify, Category = "Viewmodel|Job")
	float GetPercentageTowardsNextLevel() const;
	
protected:
	void SetJobViewModel(UJobViewModel* Job);
	void SetJobProgress(const FJobProgressItem& JobProgress);
	
	void SetIsJobAvailable(bool InValue);
	void SetIsMainJob(bool InValue);
	void SetIsSubJob(bool InValue);
	
private:
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter="IsJobAvailable", meta=(AllowPrivateAccess=true))
	bool bJobAvailable = false;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter="IsMainJob", meta=(AllowPrivateAccess=true))
	bool bMainJob = false;
	
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter="IsSubJob", meta=(AllowPrivateAccess=true))
	bool bSubJob = false;
	
	UPROPERTY(BlueprintReadOnly, FieldNotify, meta=(AllowPrivateAccess=true))
	TObjectPtr<UJobViewModel> JobViewModel;
	
	/** Cached value of the ProgressItem. */
	UPROPERTY()
	FJobProgressItem JobProgressItem;
	
	friend class UHeroJobSystemViewModel;
};

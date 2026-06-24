// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "JobSystem/HeroJobTypes.h"
#include "UI/ViewModel/CrysViewModel.h"
#include "HeroJobSystemViewModel.generated.h"

class UJobViewModel;
class UHeroJobSystemComponent;
class UHeroJobViewModel;

/**
 * Allows the switching of the Jobs in the HeroJobSystemComponent and has information of the JobProgressItems.
 */
UCLASS()
class CRYSPROJECT_API UHeroJobSystemViewModel : public UMVVMViewModelBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Viewmodel|Job")
	void SetHeroJobSystemComponent(UHeroJobSystemComponent* HeroJobSystem);
	
	UFUNCTION(BlueprintCallable, Category = "Viewmodel|Job")
	UHeroJobViewModel* FindOrCreateHeroJobViewModel(TSoftObjectPtr<UJobDefinition> JobDefinition);
	
	UJobViewModel* GetRaceViewModel() const { return RaceViewModel; }
	UHeroJobViewModel* GetMainJobViewModel() const { return MainJobViewModel; }
	UHeroJobViewModel* GetSubJobViewModel() const { return SubJobViewModel; }
	
	UFUNCTION(BlueprintPure, FieldNotify, Category = "Viewmodel|Job")
	int32 GetLevel() const { return HeroJobSystemData.Level; }
	UFUNCTION(BlueprintPure, FieldNotify, Category = "Viewmodel|Job")
	int32 GetExperience() const { return HeroJobSystemData.Experience; }
	UFUNCTION(BlueprintPure, FieldNotify, Category = "Viewmodel|Job")
	int32 GetMaxLevel() const { return HeroJobSystemData.MaxLevel; }
	UFUNCTION(BlueprintPure, FieldNotify, Category = "Viewmodel|Job")
	int32 GetMaxJobLevel() const { return HeroJobSystemData.MaxJobLevel; }
	UFUNCTION(BlueprintPure, FieldNotify, Category = "Viewmodel|Job")
	bool IsSubJobUnlocked() const { return HeroJobSystemData.bSubJobUnlocked; }
	
	UFUNCTION(BlueprintPure, FieldNotify, Category = "Viewmodel|Job")
	int32 GetExperienceRequiredUntilNextLevel() const;
	UFUNCTION(BlueprintPure, FieldNotify, Category = "Viewmodel|Job")
	float GetPercentageTowardsNextLevel() const;
	
	/** Tries to switch to specified Job. */
	UFUNCTION(BlueprintCallable, Category = "Viewmodel|Job")
	void TrySetJobs(UHeroJobViewModel* InMainJobViewModel, UHeroJobViewModel* InSubJobViewModel);
	
	UFUNCTION(BlueprintCallable, Category = "Viewmodel|Job")
	void TrySetMainJob(UHeroJobViewModel* JobViewModel);
	
	UFUNCTION(BlueprintCallable, Category = "Viewmodel|Job")
	void TrySetSubJob(UHeroJobViewModel* JobViewModel);
	
	bool IsSwitchingJobs() const { return bSwitchingJobs; }
	
protected:
	void SetRaceViewModel(UJobViewModel* NewValue);
	void SetMainJobViewModel(UHeroJobViewModel* NewValue);
	void SetSubJobViewModel(UHeroJobViewModel* NewValue);
	
	UHeroJobViewModel* CreateEmptyJobViewModel();
	
private:
	UPROPERTY(BlueprintReadOnly, FieldNotify, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UJobViewModel> RaceViewModel;

	/** The current MainJob the player is. */
	UPROPERTY(BlueprintReadOnly, FieldNotify, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UHeroJobViewModel> MainJobViewModel;
	
	/** The current SubJob the player is. */
	UPROPERTY(BlueprintReadOnly, FieldNotify, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UHeroJobViewModel> SubJobViewModel;
	
	/** True if waiting to switch Jobs. */
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter = "IsSwitchingJobs", meta = (AllowPrivateAccess = "true"))
	bool bSwitchingJobs = false;
	
	FHeroJobSystemData HeroJobSystemData;
	
	/** All the ViewModels requested to be created. The MainJob and SubJob will always be present in this array. */
	UPROPERTY()
	TArray<TObjectPtr<UHeroJobViewModel>> HeroJobViewModels;
	
	UPROPERTY()
	TObjectPtr<UHeroJobSystemComponent> HeroJobSystemComponent;
	
	UHeroJobViewModel* CreateHeroJobViewModel(TSoftObjectPtr<UJobDefinition> JobDefinition);
	
	UFUNCTION()
	void OnRaceChanged(UJobDefinition* RaceDefinition);
	
	UFUNCTION()
	void OnMainJobChanged(UJobDefinition* JobDefinition);
	
	UFUNCTION()
	void OnSubJobChanged(UJobDefinition* JobDefinition);

	UFUNCTION()
	void OnTrySetJob(bool bSuccess);

	UFUNCTION()
	void OnJobProgressUpdated(const FJobProgressItem& JobProgressItem);
	
	UFUNCTION()
	void OnHeroJobSystemDataUpdated(const FHeroJobSystemData& Data);
	
	void InitializeStartingData();
};

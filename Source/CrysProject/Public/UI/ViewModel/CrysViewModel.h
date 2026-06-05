// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "Types/MVVMEventField.h"
#include "CrysViewModel.generated.h"

struct FMVVMEventField;
/**
 * The base ViewModel that takes a PlayerController as input.
 */
UCLASS(Abstract)
class CRYSPROJECT_API UCrysViewModel : public UMVVMViewModelBase
{
	GENERATED_BODY()
	
public:
	/** Initializes the ViewModel with the PlayerController. */
	virtual void InitializeViewModel(APlayerController* PlayerController);
	
	/** Bind to this if you just need to know when a viewmodel is set. */
	UFUNCTION(BlueprintPure, FieldNotify)
	FMVVMEventField OnViewModelSet() const { return FMVVMEventField{}; }

protected:
	/**
	 * Called in InitializeViewModel. PlayerController is guaranteed to be valid.
	 */
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "InitializeViewModel")
	void K2_OnInitializeViewModel(APlayerController* PlayerController);
	
	UFUNCTION()
	virtual void OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn);
	
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "OnPossessedPawnChanged")
	void K2_OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn);
};

// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "Input/AbilityInputMode.h"
#include "UI/ViewModel/CrysViewModel.h"
#include "AbilityInputManagerViewModel.generated.h"


class UAbilityViewModel;
class AHeroPlayerController;
struct FAbilityInputSlot;
struct FAbilityInputInstance;
class UAbilityInputManagerComponent;
class UAbilityInputSlotViewModel;

USTRUCT()
struct FAbilityInputSetViewModel
{
	GENERATED_BODY()
	
	int32 InputSet = -1;
	
	UPROPERTY()
	TArray<TObjectPtr<UAbilityInputSlotViewModel>> ViewModels;
};

/**
 * Sets up listening to AbilityInputs and InputModes from the HeroPlayerController.
 */
UCLASS()
class CRYSPROJECT_API UAbilityInputManagerViewModel : public UCrysViewModel
{
	GENERATED_BODY()
	
public:
	virtual void InitializeViewModel(APlayerController* PlayerController) override;
	
	int32 GetActiveInputSet() const { return ActiveInputSet; }
	EAbilityInputMode GetInputMode() const { return InputMode; }
	
	UFUNCTION(BlueprintCallable, Category = "Viewmodel|Ability")
	UAbilityInputSlotViewModel* FindOrCreateInputSlotViewModel(const FAbilityInputSlot& InputSlot, const int32 InputSet);
	
protected:
	UFUNCTION()
	void SetActiveInputSet(const int32 InValue);
	UFUNCTION()
	void SetInputMode(const EAbilityInputMode InValue);
	
	UAbilityInputSlotViewModel* FindInputSlotViewModel(const FAbilityInputSlot& InputSlot, const int32 InputSet);
	
private:
	UPROPERTY()
	TObjectPtr<UAbilityInputManagerComponent> AbilityInputManagerComponent;
	
	// All the view models that were created to be monitored by the view model.
	UPROPERTY()
	TArray<FAbilityInputSetViewModel> AbilityInputSetViewModels;
	
	// Cached HeroPC.
	UPROPERTY()
	TObjectPtr<AHeroPlayerController> HeroPlayerController;
	
	// Reusable VM to denote an empty ability slot.
	UPROPERTY()
	TObjectPtr<UAbilityViewModel> EmptyAbilityViewModel;
	
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Category = "Viewmodel|Ability", meta = (AllowPrivateAccess = true))
	int32 ActiveInputSet = -1;
	
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Category = "Viewmodel|Ability", meta = (AllowPrivateAccess = true))
	EAbilityInputMode InputMode;
	
	void InitAbilityInputManager(APlayerController* PlayerController);
	
	UFUNCTION()
	void OnAbilityInputAdded(const FAbilityInputInstance& AbilityInputInstance, const int32 InputSet);
	UFUNCTION()
	void OnAbilityInputChanged(const FAbilityInputInstance& AbilityInputInstance, const int32 InputSet);
	UFUNCTION()
	void OnAbilityInputRemoved(const FAbilityInputInstance& AbilityInputInstance, const int32 InputSet);
};

// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "UI/ViewModel/CrysViewModel.h"
#include "ActionManagerViewModel.generated.h"

enum class ECrysActionInputMode : uint8;
class UCrysActionManagerComponent;
class UActionSlotViewModel;
class UActionViewModel;
struct FGameplayTag;
class UCrysAction;

USTRUCT()
struct FActionViewModelContainer
{
	GENERATED_BODY()
	
	UPROPERTY()
	TArray<TObjectPtr<UActionSlotViewModel>> InputTagActionViewModels;
	
	UPROPERTY()
	TArray<TObjectPtr<UActionSlotViewModel>> ActionViewModels;
};

/**
 * 
 */
UCLASS()
class CRYSPROJECT_API UActionManagerViewModel : public UCrysViewModel
{
	GENERATED_BODY()
	
public:
	virtual void InitializeViewModel(APlayerController* PlayerController) override;
	
	int32 GetActiveActionSetIndex() const { return ActiveActionSetIndex; }
	ECrysActionInputMode GetInputMode() const { return InputMode; }
	
	/** 
	 * Finds or creates an ActionViewModel from InputTag/ActionIndex from the specified ActionSetIndex. 
	 * If InputTag is invalid, checks the ActionIndex, if that is less than 0. Then no actions will be found or created.
	 */
	UFUNCTION(BlueprintCallable, Category = "Viewmodel|ActionBar")
	UActionSlotViewModel* FindOrCreateActionSlotViewModel(const FGameplayTag& InputTag, const int32 ActionIndex, const int32 ActionSetIndex);
	
	/** Finds or creates an ActionVM from InputTag from the ActiveActionSetIndex. */
	UFUNCTION(BlueprintCallable, Category = "Viewmodel|ActionBar")
	UActionSlotViewModel* FindOrCreateActiveActionSlotViewModel(const FGameplayTag& InputTag, const int32 ActionIndex);
	
	UFUNCTION(BlueprintPure, Category = "ViewModel|ActionBar")
	UCrysActionManagerComponent* GetActionManagerComponent() const { return ActionManagerComponent; }

protected:
	void InitActionManager(APlayerController* PlayerController);
	
	UFUNCTION()
	void SetActiveActionSetIndex(const int32 InActionSetIndex);
	UFUNCTION()
	void SetInputMode(ECrysActionInputMode Value);

private:
	UPROPERTY()
	TObjectPtr<UCrysActionManagerComponent> ActionManagerComponent;
	
	UPROPERTY()
	TArray<FActionViewModelContainer> ActionViewModelContainers;
	
	/** View models are updates as the active action set index changes. */
	UPROPERTY()
	FActionViewModelContainer ActiveActionViewModelContainer;
	
	/** The current set that is mapped to the InputActions. */
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Category = "Viewmodel|ActionBar", meta = (AllowPrivateAccess = true))
	int32 ActiveActionSetIndex = 0;
	
	/** The current input mode the ActionManager is in. */
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Category = "Viewmodel|ActionBar", meta = (AllowPrivateAccess = true))
	ECrysActionInputMode InputMode;
	
	UActionSlotViewModel* FindOrCreateActionSlotViewModelInternal(FActionViewModelContainer& ActionViewModelContainer, const FGameplayTag& InputTag, const int32 ActionIndex, const int32 ActionSetIndex);
	
	UActionViewModel* CreateActionViewModelInternal(const FGameplayTag& InputTag, const int32 ActionIndex, const int32 ActionSetIndex);
	
	UFUNCTION()
	void OnActionMapUpdated(UCrysAction* Action, const FGameplayTag& InputTag, const int32 ActionIndex, const int32 ActionSetIndex);
	
	/** Sets a new viewmodel for the ActionViewModelContainer. */
	void UpdateActionViewModelInternal(FActionViewModelContainer& ActionViewModelContainer, const FGameplayTag& InputTag, const int32 ActionIndex, const int32 ActionSetIndex);
};

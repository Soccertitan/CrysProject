// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "UI/ViewModel/CrysViewModel.h"
#include "ActionManagerViewModel.generated.h"

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
	TArray<TObjectPtr<UActionSlotViewModel>> Items;
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
	
	/** Finds or creates an ActionVM from InputTag and Index. */
	UFUNCTION(BlueprintCallable, Category = "Viewmodel|ActionBar")
	UActionSlotViewModel* FindOrCreateActionSlotViewModel(const FGameplayTag& InputTag, int32 Index = 0);
	
	/** Finds or creates an ActionVM from InputTag from the ActiveActionSetIndex. */
	UFUNCTION(BlueprintCallable, Category = "Viewmodel|ActionBar")
	UActionSlotViewModel* FindOrCreateActiveActionSlotViewModel(const FGameplayTag& InputTag);
	
	UFUNCTION(BlueprintCallable, Category = "Viewmodel|ActionBar")
	void SetAction(const FGameplayTag& InputTag, const int32 Index, const TSoftClassPtr<UCrysAction> ActionClass);
	
	UFUNCTION(BlueprintCallable, Category = "CrysActionManager")
	void ClearAction(const FGameplayTag& InputTag, const int32 Index);

protected:
	void InitActionManager(APlayerController* PlayerController);
	
	UFUNCTION()
	void SetActiveActionSetIndex(int32 Index);

private:
	UPROPERTY()
	TObjectPtr<UCrysActionManagerComponent> ActionManagerComponent;
	
	UPROPERTY()
	TArray<FActionViewModelContainer> ActionSlotViewModelContainers;
	
	UPROPERTY()
	TArray<TObjectPtr<UActionSlotViewModel>> ActiveActionSlotViewModels;
	
	/** The current set that is mapped to the InputActions. */
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Category = "Viewmodel|ActionBar", meta = (AllowPrivateAccess = true))
	int32 ActiveActionSetIndex = 0;
	
	UActionViewModel* InternalCreateActionViewModel(const FGameplayTag& InputTag, int32 Index);
	
	UFUNCTION()
	void OnActionMapUpdated(UCrysAction* Action, const FGameplayTag& InputTag, int32 Index);
};

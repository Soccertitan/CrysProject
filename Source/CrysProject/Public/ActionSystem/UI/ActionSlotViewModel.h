// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "MVVMViewModelBase.h"
#include "ActionSlotViewModel.generated.h"

class UActionViewModel;

/**
 * A wrapper for an ActionViewModel that defines the slot.
 */
UCLASS()
class CRYSPROJECT_API UActionSlotViewModel : public UMVVMViewModelBase
{
	GENERATED_BODY()
	
public:
	UActionViewModel* GetActionViewModel() const {return ActionViewModel;}
	
	UFUNCTION(BlueprintCallable)
	void TryActivateAction();
	
protected:
	void SetActionViewModel(UActionViewModel* Value);
	
private:
	UPROPERTY()
	FGameplayTag InputTag;
	
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Category = "Viewmodel|ActionBar", meta = (AllowPrivateAccess = true))
	TObjectPtr<UActionViewModel> ActionViewModel;
	
	friend class UActionManagerViewModel;
};

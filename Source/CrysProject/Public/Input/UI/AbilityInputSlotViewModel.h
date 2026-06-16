// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "Input/AbilityInputSlot.h"
#include "AbilityInputSlotViewModel.generated.h"

class UAbilityViewModel;
/**
 * A viewmodel with the AbilityInputSlot and the AbilityViewModel.
 */
UCLASS()
class CRYSPROJECT_API UAbilityInputSlotViewModel : public UMVVMViewModelBase
{
	GENERATED_BODY()
	
public:
	FAbilityInputSlot GetInputSlot() const { return InputSlot; }
	UAbilityViewModel* GetAbilityViewModel() const { return AbilityViewModel; }
	
protected:
	void SetInputSlot(const FAbilityInputSlot& InValue);
	void SetInputSet(const int32 InValue);
	void SetAbilityViewModel(UAbilityViewModel* InValue);
	
private:
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Category = "Viewmodel|Ability", meta = (AllowPrivateAccess = true))
	FAbilityInputSlot InputSlot;
	
	int32 InputSet = -1;
	
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Category = "Viewmodel|Ability", meta = (AllowPrivateAccess = true))
	TObjectPtr<UAbilityViewModel> AbilityViewModel;
	
	friend class UAbilityInputManagerViewModel;
};

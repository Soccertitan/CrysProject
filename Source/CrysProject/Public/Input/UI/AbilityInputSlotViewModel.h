// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "Input/AbilityInputSlot.h"
#include "AbilityInputSlotViewModel.generated.h"

class UCrysAbilityViewModel;
/**
 * A viewmodel with the AbilityInputSlot and the AbilityViewModel.
 */
UCLASS()
class CRYSPROJECT_API UAbilityInputSlotViewModel : public UMVVMViewModelBase
{
	GENERATED_BODY()
	
public:
	FAbilityInputSlot GetInputSlot() const { return InputSlot; }
	UCrysAbilityViewModel* GetAbilityViewModel() const { return AbilityViewModel; }
	
	/** Tries to activate the ability from this slot. */
	UFUNCTION(BlueprintCallable, Category = "Viewmodel|Ability")
	void InputSlotPressed() const;
	
	/** Tries to activate the ability from this slot. */
	UFUNCTION(BlueprintCallable, Category = "Viewmodel|Ability")
	void InputSlotReleased() const;
	
protected:
	void SetInputSlot(const FAbilityInputSlot& InValue);
	void SetInputSet(const int32 InValue);
	void SetAbilityViewModel(UCrysAbilityViewModel* InValue);
	
private:
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Category = "Viewmodel|Ability", meta = (AllowPrivateAccess = true))
	FAbilityInputSlot InputSlot;
	
	int32 InputSet = -1;
	
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Category = "Viewmodel|Ability", meta = (AllowPrivateAccess = true))
	TObjectPtr<UCrysAbilityViewModel> AbilityViewModel;
	
	friend class UAbilityInputManagerViewModel;
};

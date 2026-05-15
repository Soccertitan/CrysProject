// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "AttributeFractionViewModel.generated.h"

struct FGameplayTag;
class UAbilitySystemComponent;
class UAttributeViewModel;

/**
 * Takes two attributes and treats them as a Numerator/Denominator value relationship. For ease in getting percentages
 * between the current values of both. Most common example is HitPoints.
 */
UCLASS()
class CRYSPROJECT_API UAttributeFractionViewModel : public UMVVMViewModelBase
{
	GENERATED_BODY()
	
public:
	UAttributeFractionViewModel();

	UFUNCTION(BlueprintCallable, Category = "Viewmodel|Attribute")
	void SetAttributesWithASC(
		UPARAM(meta = (Categories = "Attribute")) const FGameplayTag NumeratorAttributeTag,
		UPARAM(meta = (Categories = "Attribute")) const FGameplayTag DenominatorAttributeTag,
		UAbilitySystemComponent* InAbilitySystemComponent);

	UFUNCTION(BlueprintCallable, Category = "Viewmodel|Attribute")
	void SetAttributes(UPARAM(meta = (Categories = "Attribute"))const FGameplayTag NumeratorAttributeTag, const float NumeratorCurrentValue, const float NumeratorBaseValue,
		UPARAM(meta = (Categories = "Attribute")) const FGameplayTag DenominatorAttributeTag, const float DenominatorCurrentValue, const float DenominatorBaseValue);

	UAttributeViewModel* GetNumeratorAttribute() const {return NumeratorAttribute;}
	UAttributeViewModel* GetDenominatorAttribute() const {return DenominatorAttribute;}
	
	UFUNCTION(BlueprintPure, FieldNotify, Category = "Viewmodel|Attribute")
	float GetPercentCurrentValue() const;

private:
	/** The numerator value of the attribute. */
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAttributeViewModel> NumeratorAttribute;

	/** The denominator value for the current attribute. */
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAttributeViewModel> DenominatorAttribute;

	void CreateViewModelsAndBindToDelegates();
	void BroadcastValueChanged(UObject* Object, UE::FieldNotification::FFieldId FieldId);
};

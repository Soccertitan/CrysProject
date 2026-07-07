// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "AttributeFractionViewModel.generated.h"

struct FGameplayTag;
class UAbilitySystemComponent;
class UAttributeViewModel;

/**
 * Takes two attributes and treats them as a Dividend/Divisor value relationship. For ease in getting percentages
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
		UPARAM(meta = (Categories = "Attribute")) const FGameplayTag DividendAttributeTag,
		UPARAM(meta = (Categories = "Attribute")) const FGameplayTag DivisorAttributeTag,
		UAbilitySystemComponent* InAbilitySystemComponent);

	UFUNCTION(BlueprintCallable, Category = "Viewmodel|Attribute")
	void SetAttributes(UPARAM(meta = (Categories = "Attribute"))const FGameplayTag DividendAttributeTag, const float DividendCurrentValue, const float DividendBaseValue,
		UPARAM(meta = (Categories = "Attribute")) const FGameplayTag DivisorAttributeTag, const float DivisorCurrentValue, const float DivisorBaseValue);

	UAttributeViewModel* GetDividendAttribute() const {return DividendAttribute;}
	UAttributeViewModel* GetDivisorAttribute() const {return DivisorAttribute;}
	
	UFUNCTION(BlueprintPure, FieldNotify, Category = "Viewmodel|Attribute")
	float GetPercentCurrentValue() const;

private:
	/** The numerator value of the attribute. */
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAttributeViewModel> DividendAttribute;

	/** The denominator value for the current attribute. */
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAttributeViewModel> DivisorAttribute;

	void CreateViewModelsAndBindToDelegates();
	void BroadcastValueChanged(UObject* Object, UE::FieldNotification::FFieldId FieldId);
};

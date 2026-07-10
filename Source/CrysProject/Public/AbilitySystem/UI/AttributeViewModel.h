// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "System/CrysGameplayTagRelationship.h"
#include "AttributeViewModel.generated.h"

struct FGameplayTagInfoFragment_UI;
enum class EGameplayModEvaluationChannel : uint8;
struct FOnAttributeChangeData;
class UAbilitySystemComponent;
struct FGameplayTag;

/**
 * Takes an AttributeTag and displays information about that attribute.
 */
UCLASS()
class CRYSPROJECT_API UAttributeViewModel : public UMVVMViewModelBase
{
	GENERATED_BODY()
	
public:
	/** Will bind to the ASC and listen for attribute changes on the character. */
	UFUNCTION(BlueprintCallable, Category = "Viewmodel|Attribute")
	void SetAttributeWithASC(UPARAM(meta = (Categories = "Attribute")) const FGameplayTag AttributeTag, UAbilitySystemComponent* InAbilitySystemComponent);

	/** Manually initializes the ViewModel with static data. */
	UFUNCTION(BlueprintCallable, Category = "Viewmodel|Attribute")
	void SetAttribute(UPARAM(meta = (Categories = "Attribute")) const FGameplayTag AttributeTag, float InCurrentValue, float InBaseValue);
	
	/** 
	 * If the attribute was set with an ASC. You can evaluate the attribute up to the specified channel. If there is no
	 * ASC, returns the current value.
	 */
	UFUNCTION(BlueprintPure, Category = "Viewmodel|Attribute")
	float EvaluateAttributeValueUpToChannel(EGameplayModEvaluationChannel Channel) const;
	UFUNCTION(BlueprintPure, Category = "ViewModel|Attribute")
	FText EvaluateAttributeValueTextUpToChannel(EGameplayModEvaluationChannel Channel) const;

	float GetCurrentValue() const {return CurrentValue;}
	UFUNCTION(BlueprintPure, FieldNotify, Category = "Viewmodel|Attribute")
	FText GetCurrentValueText() const;
	float GetBaseValue() const {return BaseValue;}
	UFUNCTION(BlueprintPure, FieldNotify, Category = "Viewmodel|Attribute")
	FText GetBaseValueText() const;

	UFUNCTION(BlueprintPure, FieldNotify, Category = "Viewmodel|Attribute")
	FText GetAttributeName() const;
	UFUNCTION(BlueprintPure, FieldNotify, Category = "Viewmodel|Attribute")
	FText GetShortName() const;
	UFUNCTION(BlueprintPure, FieldNotify, Category = "Viewmodel|Attribute")
	FText GetDescription() const;
	UFUNCTION(BlueprintPure, FieldNotify, Category = "Viewmodel|Attribute")
	TSoftObjectPtr<UTexture2D> GetIcon() const;
	
	FGameplayTag GetAttributeTag() const {return GameplayTagInfo.Tag;}
	
protected:
	void SetCurrentValue(float InValue);
	void SetBaseValue(float InValue);

	UFUNCTION(BlueprintPure, Category = "Viewmodel|Attribute")
	UAbilitySystemComponent* GetAbilitySystemComponent() const {return AbilitySystemComponent;}

private:
	/** The static details of the attribute. */
	UPROPERTY()
	FCrysGameplayTagInfo GameplayTagInfo;

	/** Cached ASC */
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	/** The current value of the attribute. With all modifiers */
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Category = "Viewmodel|Attribute", meta = (AllowPrivateAccess = true))
	float CurrentValue = 0.f;
	/** The base value of the attribute. */
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Category = "Viewmodel|Attribute", meta = (AllowPrivateAccess = true))
	float BaseValue = 0.f;

	void FindAndSetGameplayTagTagInfo(const FGameplayTag& AttributeTag);
	void OnAttributeValueChanged(const FOnAttributeChangeData& Data);
	FText GetValueText(float Value) const;
};

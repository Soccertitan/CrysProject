// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "AbilitySystem/AttributeTagRelationship.h"
#include "UI/UITagRelationship.h"
#include "AttributeViewModel.generated.h"

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

	float GetCurrentValue() const {return CurrentValue;}
	float GetBaseValue() const {return BaseValue;}

	UFUNCTION(BlueprintPure, FieldNotify, Category = "Viewmodel|Attribute")
	FText GetAttributeName() const {return UITagInfo.Name;}
	UFUNCTION(BlueprintPure, FieldNotify, Category = "Viewmodel|Attribute")
	FText GetShortName() const {return UITagInfo.ShortName;}
	UFUNCTION(BlueprintPure, FieldNotify, Category = "Viewmodel|Attribute")
	FText GetDescription() const {return UITagInfo.Description;}
	UFUNCTION(BlueprintPure, FieldNotify, Category = "Viewmodel|Attribute")
	bool IsAttributePercentValue() const {return AttributeTagInfo.bDisplayValueAsPercent;}
	UFUNCTION(BlueprintPure, FieldNotify, Category = "Viewmodel|Attribute")
	TSoftObjectPtr<UTexture2D> GetIcon() const {return UITagInfo.Icon;}
	
protected:
	void SetCurrentValue(float InValue);
	void SetBaseValue(float InValue);

	UFUNCTION(BlueprintPure, Category = "Viewmodel|Attribute")
	UAbilitySystemComponent* GetAbilitySystemComponent() const {return AbilitySystemComponent;}

private:
	/** The static details of the attribute. */
	UPROPERTY()
	FAttributeTagInfo AttributeTagInfo;
	UPROPERTY()
	FUITagInfo UITagInfo;

	/** Cached ASC */
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	/** The current value of the attribute. With all modifiers */
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Category = "Viewmodel|Attribute", meta = (AllowPrivateAccess = true))
	float CurrentValue = 0.f;
	/** The base value of the attribute. */
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Category = "Viewmodel|Attribute", meta = (AllowPrivateAccess = true))
	float BaseValue = 0.f;

	void FindAndSetAttributeTagInfo(const FGameplayTag& AttributeTag);
	void OnAttributeValueChanged(const FOnAttributeChangeData& Data);
};

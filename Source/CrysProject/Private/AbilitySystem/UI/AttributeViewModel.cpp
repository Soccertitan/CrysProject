// Copyright Soccertitan 2026


#include "AbilitySystem/UI/AttributeViewModel.h"

#include "AbilitySystemComponent.h"
#include "CrimAbilitySystemBlueprintFunctionLibrary.h"
#include "CrysBlueprintFunctionLibrary.h"
#include "CrysLogChannels.h"
#include "AbilitySystem/GameplayTagInfoFragment_Attribute.h"
#include "Settings/CrysGameData.h"
#include "UI/GameplayTagInfoFragment_NumberFormattingOptions.h"


void UAttributeViewModel::SetAttributeWithASC(const FGameplayTag InAttributeTag, UAbilitySystemComponent* InAbilitySystemComponent)
{
	if (!InAbilitySystemComponent)
	{
		UE_LOG(LogCrys, Error, TEXT("Invalid AbilitySystemComponent passed into [%s]"), *GetNameSafe(this));
		return;
	}

	FindAndSetGameplayTagTagInfo(InAttributeTag);
	
	if (!Fragment_Attribute.GameplayAttribute.IsValid())
	{
		UE_LOG(LogCrys, Error, TEXT("Invalid GameplayAttribute found with AttributeTag [%s] in [%s]"), *InAttributeTag.ToString(), *GetDefault<UCrysGameData>()->GetName());
		return;
	}

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Fragment_Attribute.GameplayAttribute).RemoveAll(this);
	}
	AbilitySystemComponent = InAbilitySystemComponent;
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Fragment_Attribute.GameplayAttribute).AddUObject(this, &UAttributeViewModel::OnAttributeValueChanged);

	if (AbilitySystemComponent->HasAttributeSetForAttribute(Fragment_Attribute.GameplayAttribute))
	{
		float NewCurrentValue = AbilitySystemComponent->GetFilteredAttributeValue(
				   Fragment_Attribute.GameplayAttribute, FGameplayTagRequirements(Fragment_Attribute.SourceTags), FGameplayTagContainer());
		SetCurrentValue(NewCurrentValue);
		SetBaseValue(AbilitySystemComponent->GetNumericAttributeBase(Fragment_Attribute.GameplayAttribute));
	}
}

void UAttributeViewModel::SetAttribute(const FGameplayTag InAttributeTag, float InCurrentValue, float InBaseValue)
{
	FindAndSetGameplayTagTagInfo(InAttributeTag);

	SetCurrentValue(InCurrentValue);
	SetBaseValue(InBaseValue);
}

float UAttributeViewModel::EvaluateAttributeValueUpToChannel(EGameplayModEvaluationChannel Channel) const
{
	bool bSuccess = false;
	float ReturnValue = ReturnValue = UCrimAbilitySystemBlueprintFunctionLibrary::EvaluateAttributeValueWithTagsUpToChannel(
	   AbilitySystemComponent,
	   Fragment_Attribute.GameplayAttribute,
	   Channel,
	   Fragment_Attribute.SourceTags,
	   FGameplayTagContainer(),
	   bSuccess);
	
	return bSuccess ? ReturnValue : CurrentValue;
}

FText UAttributeViewModel::EvaluateAttributeValueTextUpToChannel(EGameplayModEvaluationChannel Channel) const
{
	return Fragment_NumberFormatingOptions.GetText(EvaluateAttributeValueUpToChannel(Channel));
}

FText UAttributeViewModel::GetCurrentValueText() const
{
	return Fragment_NumberFormatingOptions.GetText(CurrentValue);
}

FText UAttributeViewModel::GetBaseValueText() const
{
	return Fragment_NumberFormatingOptions.GetText(BaseValue);
}

void UAttributeViewModel::SetCurrentValue(float InValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(CurrentValue, InValue);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetCurrentValueText);
}

void UAttributeViewModel::SetBaseValue(float InValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(BaseValue, InValue);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetBaseValueText);
}

void UAttributeViewModel::FindAndSetGameplayTagTagInfo(const FGameplayTag& InAttributeTag)
{
	if (AttributeTag != InAttributeTag)
	{
		AttributeTag = InAttributeTag;
		const FCrysGameplayTagInfo* TagInfo = UCrysBlueprintFunctionLibrary::FindCrysGameplayTagInfo(AttributeTag, true);
		
		if (TagInfo)
		{
			const FGameplayTagInfoFragment_Attribute* Fragment_AttributePtr = TagInfo->FindFragmentByType<FGameplayTagInfoFragment_Attribute>();
			Fragment_Attribute = Fragment_AttributePtr ? *Fragment_AttributePtr : FGameplayTagInfoFragment_Attribute();
			
			const FGameplayTagInfoFragment_NumberFormatingOptions* Fragment_FormattingPtr = TagInfo->FindFragmentByType<FGameplayTagInfoFragment_NumberFormatingOptions>();
			Fragment_NumberFormatingOptions = Fragment_FormattingPtr ? *Fragment_FormattingPtr : FGameplayTagInfoFragment_NumberFormatingOptions();
		}
		else
		{
			Fragment_Attribute = FGameplayTagInfoFragment_Attribute();
			Fragment_NumberFormatingOptions = FGameplayTagInfoFragment_NumberFormatingOptions();
		}
	}
}

void UAttributeViewModel::OnAttributeValueChanged(const FOnAttributeChangeData& Data)
{
	if (Fragment_Attribute.SourceTags.IsEmpty())
	{
		SetCurrentValue(Data.NewValue);
	}
	else
	{
		float NewCurrentValue = AbilitySystemComponent->GetFilteredAttributeValue(
			Fragment_Attribute.GameplayAttribute, FGameplayTagRequirements(Fragment_Attribute.SourceTags), FGameplayTagContainer());
		SetCurrentValue(NewCurrentValue);
	}

	SetBaseValue(AbilitySystemComponent->GetNumericAttributeBase(Fragment_Attribute.GameplayAttribute));
}

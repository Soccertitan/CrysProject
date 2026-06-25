// Copyright Soccertitan 2026


#include "AbilitySystem/UI/AttributeViewModel.h"

#include "AbilitySystemComponent.h"
#include "CrimAbilitySystemBlueprintFunctionLibrary.h"
#include "CrysBlueprintFunctionLibrary.h"
#include "CrysLogChannels.h"
#include "Settings/CrysGameData.h"


void UAttributeViewModel::SetAttributeWithASC(const FGameplayTag AttributeTag, UAbilitySystemComponent* InAbilitySystemComponent)
{
	if (!InAbilitySystemComponent)
	{
		UE_LOG(LogCrys, Error, TEXT("Invalid AbilitySystemComponent passed into [%s]"), *GetNameSafe(this));
		return;
	}

	FindAndSetAttributeTagInfo(AttributeTag);

	if (!AttributeTagInfo.IsValid())
	{
		if (!AttributeTagInfo.GameplayAttribute.IsValid())
		{
			UE_LOG(LogCrys, Error, TEXT("Invalid GameplayAttribute found with AttributeTag [%s] in [%s]"), *AttributeTag.ToString(), *GetDefault<UCrysGameData>()->GetName());
			return;
		}
	}

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeTagInfo.GameplayAttribute).RemoveAll(this);
	}
	AbilitySystemComponent = InAbilitySystemComponent;
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeTagInfo.GameplayAttribute).AddUObject(this, &UAttributeViewModel::OnAttributeValueChanged);

	if (AbilitySystemComponent->HasAttributeSetForAttribute(AttributeTagInfo.GameplayAttribute))
	{
		float NewCurrentValue = AbilitySystemComponent->GetFilteredAttributeValue(
				   AttributeTagInfo.GameplayAttribute, FGameplayTagRequirements(AttributeTagInfo.SourceTags), FGameplayTagContainer());
		SetCurrentValue(NewCurrentValue);
		SetBaseValue(AbilitySystemComponent->GetNumericAttributeBase(AttributeTagInfo.GameplayAttribute));
	}
}

void UAttributeViewModel::SetAttribute(const FGameplayTag AttributeTag, float InCurrentValue, float InBaseValue)
{
	FindAndSetAttributeTagInfo(AttributeTag);

	SetCurrentValue(InCurrentValue);
	SetBaseValue(InBaseValue);
}

float UAttributeViewModel::EvaluateAttributeValueUpToChannel(EGameplayModEvaluationChannel Channel) const
{
	bool bSuccess = false;
	const float ReturnValue = UCrimAbilitySystemBlueprintFunctionLibrary::EvaluateAttributeValueWithTagsUpToChannel(
		AbilitySystemComponent,
		AttributeTagInfo.GameplayAttribute,
		Channel,
		AttributeTagInfo.SourceTags,
		FGameplayTagContainer(),
		bSuccess);
	
	return bSuccess ? ReturnValue : CurrentValue;
}

void UAttributeViewModel::SetCurrentValue(float InValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(CurrentValue, InValue);
}

void UAttributeViewModel::SetBaseValue(float InValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(BaseValue, InValue);
}

void UAttributeViewModel::FindAndSetAttributeTagInfo(const FGameplayTag& AttributeTag)
{
	AttributeTagInfo = UCrysBlueprintFunctionLibrary::FindAttributeTagInfo(AttributeTag, true);
	UITagInfo = UCrysBlueprintFunctionLibrary::FindUITagInfo(AttributeTag, false);
	
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetDescription);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetIcon);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetAttributeName);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetShortName);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(IsAttributePercentValue);
}

void UAttributeViewModel::OnAttributeValueChanged(const FOnAttributeChangeData& Data)
{
	if (AttributeTagInfo.SourceTags.IsEmpty())
	{
		SetCurrentValue(Data.NewValue);
	}
	else
	{
		float NewCurrentValue = AbilitySystemComponent->GetFilteredAttributeValue(
			AttributeTagInfo.GameplayAttribute, FGameplayTagRequirements(AttributeTagInfo.SourceTags), FGameplayTagContainer());
		SetCurrentValue(NewCurrentValue);
	}

	SetBaseValue(AbilitySystemComponent->GetNumericAttributeBase(AttributeTagInfo.GameplayAttribute));
}

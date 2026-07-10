// Copyright Soccertitan 2026


#include "AbilitySystem/UI/AttributeViewModel.h"

#include "AbilitySystemComponent.h"
#include "CrimAbilitySystemBlueprintFunctionLibrary.h"
#include "CrysBlueprintFunctionLibrary.h"
#include "CrysLogChannels.h"
#include "AbilitySystem/GameplayTagInfoFragment_Attribute.h"
#include "Settings/CrysGameData.h"
#include "UI/GameplayTagInfoFragment_NumberFormattingOptions.h"
#include "UI/GameplayTagInfoFragment_UI.h"


void UAttributeViewModel::SetAttributeWithASC(const FGameplayTag AttributeTag, UAbilitySystemComponent* InAbilitySystemComponent)
{
	if (!InAbilitySystemComponent)
	{
		UE_LOG(LogCrys, Error, TEXT("Invalid AbilitySystemComponent passed into [%s]"), *GetNameSafe(this));
		return;
	}

	FindAndSetGameplayTagTagInfo(AttributeTag);

	const FGameplayTagInfoFragment_Attribute* Fragment_Attribute = GameplayTagInfo.FindFragmentByType<FGameplayTagInfoFragment_Attribute>();
	if (!Fragment_Attribute || !Fragment_Attribute->GameplayAttribute.IsValid())
	{
		UE_LOG(LogCrys, Error, TEXT("Invalid GameplayAttribute found with AttributeTag [%s] in [%s]"), *AttributeTag.ToString(), *GetDefault<UCrysGameData>()->GetName());
		return;
	}

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Fragment_Attribute->GameplayAttribute).RemoveAll(this);
	}
	AbilitySystemComponent = InAbilitySystemComponent;
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Fragment_Attribute->GameplayAttribute).AddUObject(this, &UAttributeViewModel::OnAttributeValueChanged);

	if (AbilitySystemComponent->HasAttributeSetForAttribute(Fragment_Attribute->GameplayAttribute))
	{
		float NewCurrentValue = AbilitySystemComponent->GetFilteredAttributeValue(
				   Fragment_Attribute->GameplayAttribute, FGameplayTagRequirements(Fragment_Attribute->SourceTags), FGameplayTagContainer());
		SetCurrentValue(NewCurrentValue);
		SetBaseValue(AbilitySystemComponent->GetNumericAttributeBase(Fragment_Attribute->GameplayAttribute));
	}
}

void UAttributeViewModel::SetAttribute(const FGameplayTag AttributeTag, float InCurrentValue, float InBaseValue)
{
	FindAndSetGameplayTagTagInfo(AttributeTag);

	SetCurrentValue(InCurrentValue);
	SetBaseValue(InBaseValue);
}

float UAttributeViewModel::EvaluateAttributeValueUpToChannel(EGameplayModEvaluationChannel Channel) const
{
	bool bSuccess = false;
	float ReturnValue = 0.f;
	if (const FGameplayTagInfoFragment_Attribute* Fragment_Attribute = GameplayTagInfo.FindFragmentByType<FGameplayTagInfoFragment_Attribute>())
	{
		ReturnValue = UCrimAbilitySystemBlueprintFunctionLibrary::EvaluateAttributeValueWithTagsUpToChannel(
		   AbilitySystemComponent,
		   Fragment_Attribute->GameplayAttribute,
		   Channel,
		   Fragment_Attribute->SourceTags,
		   FGameplayTagContainer(),
		   bSuccess);
	}
	
	return bSuccess ? ReturnValue : CurrentValue;
}

FText UAttributeViewModel::EvaluateAttributeValueTextUpToChannel(EGameplayModEvaluationChannel Channel) const
{
	return GetValueText(EvaluateAttributeValueUpToChannel(Channel));
}

FText UAttributeViewModel::GetCurrentValueText() const
{
	return GetValueText(CurrentValue);
}

FText UAttributeViewModel::GetBaseValueText() const
{
	return GetValueText(BaseValue);
}

FText UAttributeViewModel::GetAttributeName() const
{
	if (const FGameplayTagInfoFragment_UI* Fragment = GameplayTagInfo.FindFragmentByType<FGameplayTagInfoFragment_UI>())
	{
		return Fragment->Name;
	}
	return FText();
}

FText UAttributeViewModel::GetShortName() const
{
	if (const FGameplayTagInfoFragment_UI* Fragment = GameplayTagInfo.FindFragmentByType<FGameplayTagInfoFragment_UI>())
	{
		return Fragment->ShortName;
	}
	return FText();
}

FText UAttributeViewModel::GetDescription() const
{
	if (const FGameplayTagInfoFragment_UI* Fragment = GameplayTagInfo.FindFragmentByType<FGameplayTagInfoFragment_UI>())
	{
		return Fragment->Description;
	}
	return FText();
}

TSoftObjectPtr<UTexture2D> UAttributeViewModel::GetIcon() const
{
	if (const FGameplayTagInfoFragment_UI* Fragment = GameplayTagInfo.FindFragmentByType<FGameplayTagInfoFragment_UI>())
	{
		return Fragment->Icon;
	}
	return nullptr;
}

void UAttributeViewModel::SetCurrentValue(float InValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(CurrentValue, InValue);
}

void UAttributeViewModel::SetBaseValue(float InValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(BaseValue, InValue);
}

void UAttributeViewModel::FindAndSetGameplayTagTagInfo(const FGameplayTag& AttributeTag)
{
	if (GameplayTagInfo.Tag != AttributeTag)
	{
		GameplayTagInfo = UCrysBlueprintFunctionLibrary::FindCrysGameplayTagInfo(AttributeTag, true);
	
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetDescription);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetIcon);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetAttributeName);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetShortName);
	}
}

void UAttributeViewModel::OnAttributeValueChanged(const FOnAttributeChangeData& Data)
{
	const FGameplayTagInfoFragment_Attribute* Fragment_Attribute = GameplayTagInfo.FindFragmentByType<FGameplayTagInfoFragment_Attribute>();
	
	if (Fragment_Attribute->SourceTags.IsEmpty())
	{
		SetCurrentValue(Data.NewValue);
	}
	else
	{
		float NewCurrentValue = AbilitySystemComponent->GetFilteredAttributeValue(
			Fragment_Attribute->GameplayAttribute, FGameplayTagRequirements(Fragment_Attribute->SourceTags), FGameplayTagContainer());
		SetCurrentValue(NewCurrentValue);
	}

	SetBaseValue(AbilitySystemComponent->GetNumericAttributeBase(Fragment_Attribute->GameplayAttribute));
}

FText UAttributeViewModel::GetValueText(float Value) const
{
	const FGameplayTagInfoFragment_NumberFormatingOptions* Fragment = GameplayTagInfo.FindFragmentByType<FGameplayTagInfoFragment_NumberFormatingOptions>();
	return Fragment ? Fragment->GetText(Value) : FGameplayTagInfoFragment_NumberFormatingOptions().GetText(Value);
}

// Copyright Soccertitan 2026


#include "AbilitySystem/UI/AttributeFractionViewModel.h"

#include "CrysLogChannels.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/UI/AttributeViewModel.h"


UAttributeFractionViewModel::UAttributeFractionViewModel()
{
}

void UAttributeFractionViewModel::SetAttributesWithASC(const FGameplayTag NumeratorAttributeTag, 
	const FGameplayTag DenominatorAttributeTag, UAbilitySystemComponent* InAbilitySystemComponent)
{
	if (!InAbilitySystemComponent)
	{
		UE_LOG(LogCrys, Error, TEXT("Invalid AbilitySystemComponent in [%s]"), *GetName());
		return;
	}

	CreateViewModelsAndBindToDelegates();

	NumeratorAttribute->SetAttributeWithASC(NumeratorAttributeTag, InAbilitySystemComponent);
	DenominatorAttribute->SetAttributeWithASC(DenominatorAttributeTag, InAbilitySystemComponent);
}

void UAttributeFractionViewModel::SetAttributes(const FGameplayTag NumeratorAttributeTag,
	const float NumeratorCurrentValue, const float NumeratorBaseValue, const FGameplayTag DenominatorAttributeTag,
	const float DenominatorCurrentValue, const float DenominatorBaseValue)
{
	CreateViewModelsAndBindToDelegates();

	NumeratorAttribute->SetAttribute(NumeratorAttributeTag, NumeratorCurrentValue, NumeratorBaseValue);
	DenominatorAttribute->SetAttribute(DenominatorAttributeTag, DenominatorCurrentValue, DenominatorBaseValue);
}

float UAttributeFractionViewModel::GetPercentCurrentValue() const
{
	if (!NumeratorAttribute || !DenominatorAttribute)
	{
		return 0.f;
	}

	if (FMath::IsNearlyZero(DenominatorAttribute->GetCurrentValue()))
	{
		return 0.f;
	}
	return NumeratorAttribute->GetCurrentValue() / DenominatorAttribute->GetCurrentValue();
}

void UAttributeFractionViewModel::CreateViewModelsAndBindToDelegates()
{
	FFieldValueChangedDelegate Delegate = FFieldValueChangedDelegate::CreateUObject(this, &UAttributeFractionViewModel::BroadcastValueChanged);
	if (!NumeratorAttribute)
	{
		NumeratorAttribute = NewObject<UAttributeViewModel>(this, UAttributeViewModel::StaticClass());
		NumeratorAttribute->AddFieldValueChangedDelegate(UAttributeViewModel::FFieldNotificationClassDescriptor::CurrentValue, Delegate);
	}
	if (!DenominatorAttribute)
	{
		DenominatorAttribute = NewObject<UAttributeViewModel>(this, UAttributeViewModel::StaticClass());
		DenominatorAttribute->AddFieldValueChangedDelegate(UAttributeViewModel::FFieldNotificationClassDescriptor::CurrentValue, Delegate);
	}
}

void UAttributeFractionViewModel::BroadcastValueChanged(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetPercentCurrentValue);
}
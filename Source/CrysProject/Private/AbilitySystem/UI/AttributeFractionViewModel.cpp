// Copyright Soccertitan 2026


#include "AbilitySystem/UI/AttributeFractionViewModel.h"

#include "CrysLogChannels.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/UI/AttributeViewModel.h"


UAttributeFractionViewModel::UAttributeFractionViewModel()
{
}

void UAttributeFractionViewModel::SetAttributesWithASC(const FGameplayTag DividendAttributeTag, 
	const FGameplayTag DivisorAttributeTag, UAbilitySystemComponent* InAbilitySystemComponent)
{
	if (!InAbilitySystemComponent)
	{
		UE_LOG(LogCrys, Error, TEXT("Invalid AbilitySystemComponent in [%s]"), *GetName());
		return;
	}

	CreateViewModelsAndBindToDelegates();

	DividendAttribute->SetAttributeWithASC(DividendAttributeTag, InAbilitySystemComponent);
	DivisorAttribute->SetAttributeWithASC(DivisorAttributeTag, InAbilitySystemComponent);
}

void UAttributeFractionViewModel::SetAttributes(const FGameplayTag DividendAttributeTag,
	const float DividendCurrentValue, const float DividendBaseValue, const FGameplayTag DivisorAttributeTag,
	const float DivisorCurrentValue, const float DivisorBaseValue)
{
	CreateViewModelsAndBindToDelegates();

	DividendAttribute->SetAttribute(DividendAttributeTag, DividendCurrentValue, DividendBaseValue);
	DivisorAttribute->SetAttribute(DivisorAttributeTag, DivisorCurrentValue, DivisorBaseValue);
}

float UAttributeFractionViewModel::GetPercentCurrentValue() const
{
	if (!DividendAttribute || !DivisorAttribute)
	{
		return 0.f;
	}

	if (FMath::IsNearlyZero(DivisorAttribute->GetCurrentValue()))
	{
		return 0.f;
	}
	return DividendAttribute->GetCurrentValue() / DivisorAttribute->GetCurrentValue();
}

void UAttributeFractionViewModel::CreateViewModelsAndBindToDelegates()
{
	FFieldValueChangedDelegate Delegate = FFieldValueChangedDelegate::CreateUObject(this, &UAttributeFractionViewModel::BroadcastValueChanged);
	if (!DividendAttribute)
	{
		DividendAttribute = NewObject<UAttributeViewModel>(this, UAttributeViewModel::StaticClass());
		DividendAttribute->AddFieldValueChangedDelegate(UAttributeViewModel::FFieldNotificationClassDescriptor::CurrentValue, Delegate);
	}
	if (!DivisorAttribute)
	{
		DivisorAttribute = NewObject<UAttributeViewModel>(this, UAttributeViewModel::StaticClass());
		DivisorAttribute->AddFieldValueChangedDelegate(UAttributeViewModel::FFieldNotificationClassDescriptor::CurrentValue, Delegate);
	}
}

void UAttributeFractionViewModel::BroadcastValueChanged(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetPercentCurrentValue);
}
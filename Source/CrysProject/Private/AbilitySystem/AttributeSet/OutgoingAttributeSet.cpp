// Copyright Soccertitan 2026


#include "AbilitySystem/AttributeSet/OutgoingAttributeSet.h"

UOutgoingAttributeSet::UOutgoingAttributeSet()
{
}

void UOutgoingAttributeSet::ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const
{
	NewValue = FMath::Max(NewValue, 0.f);
}

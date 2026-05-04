// Copyright Soccertitan 2026


#include "AbilitySystem/AttributeSet/MovementAttributeSet.h"

#include "Net/UnrealNetwork.h"

UMovementAttributeSet::UMovementAttributeSet()
{
	MovementSpeedMultiplier = 1.f;
}

void UMovementAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, MovementSpeedMultiplier, COND_None, REPNOTIFY_Always);
}

void UMovementAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);
	
	// Make sure we don't change the base as this is always supposed to return a value of 1.
	NewValue = 1.f;
}

void UMovementAttributeSet::ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::ClampAttribute(Attribute, NewValue);
	
	NewValue = FMath::Max(NewValue, 0.f);
}

void UMovementAttributeSet::OnRep_MovementSpeedMultiplier(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, MovementSpeedMultiplier, OldValue);
}

// Copyright Soccertitan 2026


#include "Character/CrysCharacterMovementComponent.h"

#include "CrimAbilitySystemComponent.h"
#include "CrysNativeGameplayTags.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/AttributeSet/MovementAttributeSet.h"


UCrysCharacterMovementComponent::UCrysCharacterMovementComponent()
{
	bOrientRotationToMovement = true;
}

float UCrysCharacterMovementComponent::GetMaxSpeed() const
{
	if (bMovementRooted)
	{
		return 0.f;
	}
	
	return Super::GetMaxSpeed() * MovementSpeedMultiplier;
}

FRotator UCrysCharacterMovementComponent::GetDeltaRotation(float DeltaTime) const
{
	if (bMovementRooted)
	{
		return FRotator();
	}

	return Super::GetDeltaRotation(DeltaTime);
}

void UCrysCharacterMovementComponent::InitializeWithAbilitySystem_Implementation(UCrimAbilitySystemComponent* NewAbilitySystemComponent)
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->RegisterGameplayTagEvent(
			Crys::NativeGameplayTag::Ability_State_MovementRooted,
			EGameplayTagEventType::NewOrRemoved).RemoveAll(this);
	}

	AbilitySystemComponent = NewAbilitySystemComponent;
	MovementSpeedMultiplier = 1.f;
	
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->RegisterGameplayTagEvent(
		   Crys::NativeGameplayTag::Ability_State_MovementRooted,
		   EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UCrysCharacterMovementComponent::OnGameplayTagMovementRootedUpdated);
		
		bMovementRooted = AbilitySystemComponent->HasMatchingGameplayTag(Crys::NativeGameplayTag::Ability_State_MovementRooted);
		
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UMovementAttributeSet::GetMovementSpeedMultiplierAttribute())
			.AddUObject(this, &UCrysCharacterMovementComponent::OnMovementSpeedMultiplierUpdated);
		
		bool bFound = false;
		MovementSpeedMultiplier = AbilitySystemComponent->GetGameplayAttributeValue(
			UMovementAttributeSet::GetMovementSpeedMultiplierAttribute(), bFound);
		MovementSpeedMultiplier = bFound ? MovementSpeedMultiplier : 1.f;
	}
}

void UCrysCharacterMovementComponent::OnGameplayTagMovementRootedUpdated(const FGameplayTag Tag, int32 NewCount)
{
	bMovementRooted = NewCount > 0;
}

void UCrysCharacterMovementComponent::OnMovementSpeedMultiplierUpdated(const FOnAttributeChangeData& Data)
{
	MovementSpeedMultiplier = Data.NewValue;
}

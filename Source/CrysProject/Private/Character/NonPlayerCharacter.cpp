// Copyright Soccertitan 2026


#include "Character/NonPlayerCharacter.h"

#include "AbilitySet.h"
#include "CrimAbilitySystemComponent.h"
#include "AbilitySystem/AttributeSet/AbilityAttributeSet.h"
#include "AbilitySystem/AttributeSet/AttackerAttributeSet.h"
#include "AbilitySystem/AttributeSet/CrysHitPointsAttributeSet.h"
#include "AbilitySystem/AttributeSet/DefenderAttributeSet.h"
#include "AbilitySystem/AttributeSet/JobAttributeSet.h"
#include "AbilitySystem/AttributeSet/ManaPointsAttributeSet.h"
#include "AbilitySystem/AttributeSet/MovementAttributeSet.h"
#include "AbilitySystem/AttributeSet/PrimaryAttributeSet.h"
#include "AbilitySystem/AttributeSet/TacticalPointsAttributeSet.h"
#include "Attribute/HitPointsComponent.h"


ANonPlayerCharacter::ANonPlayerCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SetNetUpdateFrequency(100.f);
	
	AbilitySystemComponent = CreateDefaultSubobject<UCrimAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	
	HitPointsAttributeSet = CreateDefaultSubobject<UCrysHitPointsAttributeSet>("HitPointsAttributeSet");
	ManaPointsAttributeSet = CreateDefaultSubobject<UManaPointsAttributeSet>("ManaPointsAttributeSet");
	TacticalPointsAttributeSet = CreateDefaultSubobject<UTacticalPointsAttributeSet>("TacticalPointsAttributeSet");
	PrimaryAttributeSet = CreateDefaultSubobject<UPrimaryAttributeSet>("PrimaryAttributeSet");
	AbilityAttributeSet = CreateDefaultSubobject<UAbilityAttributeSet>("AbilityAttributeSet");
	AttackerAttributeSet = CreateDefaultSubobject<UAttackerAttributeSet>("AttackerAttributeSet");
	DefenderAttributeSet = CreateDefaultSubobject<UDefenderAttributeSet>("DefenderAttributeSet");
	JobAttributeSet = CreateDefaultSubobject<UJobAttributeSet>("JobAttributeSet");
	MovementAttributeSet = CreateDefaultSubobject<UMovementAttributeSet>("MovementAttributeSet");
	
	HitPointsComponent = CreateDefaultSubobject<UHitPointsComponent>(TEXT("HitPointsComponent"));
	
	GenericTeamId = FGenericTeamId(2);
}

void ANonPlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	GrantAbilitySets();
}

UAbilitySystemComponent* ANonPlayerCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

FGenericTeamId ANonPlayerCharacter::GetGenericTeamId() const
{
	return GenericTeamId;
}

void ANonPlayerCharacter::SetGenericTeamId(const FGenericTeamId& TeamID)
{
	// GenericTeamId = TeamID;
}

void ANonPlayerCharacter::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	AbilitySystemComponent->GetOwnedGameplayTags(TagContainer);
}

bool ANonPlayerCharacter::HasMatchingGameplayTag(FGameplayTag TagToCheck) const
{
	return AbilitySystemComponent->HasMatchingGameplayTag(TagToCheck);
}

bool ANonPlayerCharacter::HasAllMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const
{
	return AbilitySystemComponent->HasAllMatchingGameplayTags(TagContainer);
}

bool ANonPlayerCharacter::HasAnyMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const
{
	return AbilitySystemComponent->HasAnyMatchingGameplayTags(TagContainer);
}

void ANonPlayerCharacter::GrantAbilitySets()
{
	if (HasAuthority())
	{
		for (const TObjectPtr<UAbilitySet>& AbilitySet : AbilitySets)
		{
			if (AbilitySet)
			{
				AbilitySet->GiveToAbilitySystem(AbilitySystemComponent, nullptr);
			}
		}
	}
}




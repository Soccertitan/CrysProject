// Copyright Soccertitan 2026


#include "Character/NonPlayerCharacter.h"

#include "AbilitySet.h"
#include "AIController.h"
#include "CrimAbilitySystemComponent.h"
#include "AbilitySystem/Ability/Combat/CombatSystemComponent.h"
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
#include "JobSystem/JobSystemComponent.h"


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
	JobSystemComponent = CreateDefaultSubobject<UJobSystemComponent>(TEXT("JobSystemComponent"));
	CombatSystemComponent = CreateDefaultSubobject<UCombatSystemComponent>(TEXT("CombatSystemComponent"));
	
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

UJobSystemComponent* ANonPlayerCharacter::GetJobSystemComponent_Implementation() const
{
	return JobSystemComponent;
}

UCombatSystemComponent* ANonPlayerCharacter::GetCombatSystemComponent_Implementation() const
{
	return CombatSystemComponent;
}

AActor* ANonPlayerCharacter::GetAbilityTarget_Implementation() const
{
	if (AAIController* AIController = Cast<AAIController>(GetController()))
	{
		return AIController->GetFocusActor();
	}
	return nullptr;
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




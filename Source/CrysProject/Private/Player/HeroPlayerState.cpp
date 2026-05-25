// Copyright Soccertitan 2026


#include "Player/HeroPlayerState.h"

#include "AbilitySet.h"
#include "CrimAbilitySystemComponent.h"
#include "CrimTargetingSystemBlueprintFunctionLibrary.h"
#include "CrimTargetingSystemComponent.h"
#include "CrimTargetingSystemInterface.h"
#include "InventoryManagerComponent.h"
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
#include "EquipmentSystem/EquipmentManagerComponent.h"
#include "JobSystem/HeroJobSystemComponent.h"

AHeroPlayerState::AHeroPlayerState()
{
	SetNetUpdateFrequency(100.f);
	
	AbilitySystemComponent = CreateDefaultSubobject<UCrimAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Full);
	
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
	HeroJobSystemComponent = CreateDefaultSubobject<UHeroJobSystemComponent>(TEXT("HeroJobSystemComponent"));
	CombatSystemComponent = CreateDefaultSubobject<UCombatSystemComponent>(TEXT("CombatSystemComponent"));
	
	InventoryManagerComponent = CreateDefaultSubobject<UInventoryManagerComponent>("InventoryManagerComponent");
	InventoryManagerComponent->SetIsReplicated(true);
	bReplicateUsingRegisteredSubObjectList = true;
	
	EquipmentManagerComponent = CreateDefaultSubobject<UEquipmentManagerComponent>("EquipmentManagerComponent");
	
	GenericTeamId = FGenericTeamId(1);
}

void AHeroPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	GrantAbilitySets();
}

void AHeroPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

UAbilitySystemComponent* AHeroPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UInventoryManagerComponent* AHeroPlayerState::GetInventoryManagerComponent_Implementation() const
{
	return InventoryManagerComponent;
}

UJobSystemComponent* AHeroPlayerState::GetJobSystemComponent_Implementation() const
{
	return HeroJobSystemComponent;
}

UEquipmentManagerComponent* AHeroPlayerState::GetEquipmentManagerComponent_Implementation() const
{
	return EquipmentManagerComponent;
}

UCombatSystemComponent* AHeroPlayerState::GetCombatSystemComponent_Implementation() const
{
	return CombatSystemComponent;
}

UCrimTargetingSystemComponent* AHeroPlayerState::GetCrimTargetingSystemComponent_Implementation() const
{
	return UCrimTargetingSystemBlueprintFunctionLibrary::GetCrimTargetingSystemComponent(GetOwningController(), false);
}

AActor* AHeroPlayerState::GetAbilityTarget_Implementation() const
{
	if (UCrimTargetingSystemComponent* TargetingSystemComponent = UCrimTargetingSystemBlueprintFunctionLibrary::GetCrimTargetingSystemComponent(GetOwningController(), false))
	{
		return TargetingSystemComponent->GetTargetPoint().GetActor();
	}
	return nullptr;
}

FGenericTeamId AHeroPlayerState::GetGenericTeamId() const
{
	return GenericTeamId;
}

void AHeroPlayerState::SetGenericTeamId(const FGenericTeamId& TeamID)
{
	// Prevent changing the team ID. Currently want to force hero's to always be a value of 1.
	// IGenericTeamAgentInterface::SetGenericTeamId(TeamID);
}

void AHeroPlayerState::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	AbilitySystemComponent->GetOwnedGameplayTags(TagContainer);
}

bool AHeroPlayerState::HasMatchingGameplayTag(FGameplayTag TagToCheck) const
{
	return AbilitySystemComponent->HasMatchingGameplayTag(TagToCheck);
}

bool AHeroPlayerState::HasAllMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const
{
	return AbilitySystemComponent->HasAllMatchingGameplayTags(TagContainer);
}

bool AHeroPlayerState::HasAnyMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const
{
	return AbilitySystemComponent->HasAnyMatchingGameplayTags(TagContainer);
}

void AHeroPlayerState::GrantAbilitySets()
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

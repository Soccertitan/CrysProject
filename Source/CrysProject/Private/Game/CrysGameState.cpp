// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/CrysGameState.h"

#include "AbilitySystemComponent.h"
#include "CrimAbilitySystemComponent.h"
#include "GameFramework/GameplayMessageSubsystem.h"

ACrysGameState::ACrysGameState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UCrimAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
}

void ACrysGameState::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	check(AbilitySystemComponent);
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
}

UAbilitySystemComponent* ACrysGameState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ACrysGameState::MulticastMessageToClients_Implementation(const FAbilityVerbMessage Message)
{
	if (GetNetMode() == NM_Client)
	{
		UGameplayMessageSubsystem::Get(this).BroadcastMessage(Message.Verb, Message);
	}
}

void ACrysGameState::MulticastReliableMessageToClients_Implementation(const FAbilityVerbMessage Message)
{
	MulticastMessageToClients_Implementation(Message);
}

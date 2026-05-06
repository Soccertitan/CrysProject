// Copyright Soccertitan 2026


#include "Character/HeroCharacter.h"

#include "CrimAbilitySystemBlueprintFunctionLibrary.h"
#include "CrimAbilitySystemComponent.h"
#include "InventoryBlueprintFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/SpringArmComponent.h"


AHeroCharacter::AHeroCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->ProbeChannel = ECC_Visibility;
	CameraBoom->bUsePawnControlRotation = true; // Rotates the arm based on the controller
	CameraBoom->TargetArmLength = 500.0f;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	// Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	// Camera does not rotate relative to arm
	FollowCamera->bUsePawnControlRotation = false;
}

void AHeroCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	InitAbilitySystem();
}

void AHeroCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	InitAbilitySystem();
}

UAbilitySystemComponent* AHeroCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UInventoryManagerComponent* AHeroCharacter::GetInventoryManagerComponent_Implementation() const
{
	return UInventoryBlueprintFunctionLibrary::GetInventoryManagerComponent(GetPlayerState());
}

FGenericTeamId AHeroCharacter::GetGenericTeamId() const
{
	if (IGenericTeamAgentInterface* Interface = Cast<IGenericTeamAgentInterface>(GetPlayerState()))
	{
		return Interface->GetGenericTeamId();
	}
	return FGenericTeamId();
}

void AHeroCharacter::SetGenericTeamId(const FGenericTeamId& TeamID)
{
	if (IGenericTeamAgentInterface* Interface = Cast<IGenericTeamAgentInterface>(GetPlayerState()))
	{
		Interface->SetGenericTeamId(TeamID);
	}
}

void AHeroCharacter::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	AbilitySystemComponent->GetOwnedGameplayTags(TagContainer);
}

bool AHeroCharacter::HasMatchingGameplayTag(FGameplayTag TagToCheck) const
{
	return AbilitySystemComponent->HasMatchingGameplayTag(TagToCheck);
}

bool AHeroCharacter::HasAllMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const
{
	return AbilitySystemComponent->HasAllMatchingGameplayTags(TagContainer);
}

bool AHeroCharacter::HasAnyMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const
{
	return AbilitySystemComponent->HasAnyMatchingGameplayTags(TagContainer);
}

void AHeroCharacter::InitAbilitySystem()
{
	AbilitySystemComponent = UCrimAbilitySystemBlueprintFunctionLibrary::GetAbilitySystemComponent(GetPlayerState());
	check(AbilitySystemComponent);
	AbilitySystemComponent->InitAbilityActorInfo(GetPlayerState(), this);
}




// Copyright Soccertitan 2026


#include "Player/HeroPlayerController.h"

#include "CrimAbilitySystemBlueprintFunctionLibrary.h"
#include "CrimTargetingSystemComponent.h"
#include "CrysLogChannels.h"
#include "EnhancedInputSubsystems.h"
#include "Input/AbilityInputManagerComponent.h"
#include "Player/HeroPlayerState.h"

AHeroPlayerController::AHeroPlayerController()
{
	AbilityInputManagerComponent = CreateDefaultSubobject<UAbilityInputManagerComponent>(TEXT("AbilityInputManagerComponent"));
	AbilityInputManagerComponent->SetIsReplicated(false);
	
	TargetingSystemComponent = CreateDefaultSubobject<UCrimTargetingSystemComponent>(TEXT("CrimTargetingSystemComponent"));
}

UCrimTargetingSystemComponent* AHeroPlayerController::GetCrimTargetingSystemComponent_Implementation() const
{
	return TargetingSystemComponent;
}

void AHeroPlayerController::SetAbilityInputMode(const EAbilityInputMode InputMode)
{
	if (InputMode != GetAbilityInputMode())
	{
		InternalSetAbilityInputMode(InputMode);
	}
}

void AHeroPlayerController::AcknowledgePossession(class APawn* P)
{
	Super::AcknowledgePossession(P);
	
	InitAbilitySystemComponent();
}

void AHeroPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	InitAbilitySystemComponent();
}

void AHeroPlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	AbilityInputManagerComponent->ProcessAbilityInput();
	
	Super::PostProcessInput(DeltaTime, bGamePaused);
}

void AHeroPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	InitAbilitySystemComponent();
}

void AHeroPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	InternalSetAbilityInputMode(AbilityInputMode);
}

void AHeroPlayerController::InitAbilitySystemComponent()
{
	AbilitySystemComponent = UCrimAbilitySystemBlueprintFunctionLibrary::GetAbilitySystemComponent(
		GetPlayerState<AHeroPlayerState>(), true);
	if (AbilitySystemComponent)
	{
		ICrimAbilitySystemInterface::Execute_SetCrimAbilitySystem(AbilityInputManagerComponent, AbilitySystemComponent);
	}
}

void AHeroPlayerController::InternalSetAbilityInputMode(const EAbilityInputMode InputMode)
{
	if (EnhancedInputSubsystem)
	{
		if (AbilityInputModeInputMappingContextMap.Contains(AbilityInputMode))
		{
			EnhancedInputSubsystem->RemoveMappingContext(*AbilityInputModeInputMappingContextMap.Find(AbilityInputMode));
		}
		
		AbilityInputMode = InputMode;
		OnAbilityInputModeChangedDelegate.Broadcast(AbilityInputMode);
		
		if (AbilityInputModeInputMappingContextMap.Contains(AbilityInputMode))
		{
			EnhancedInputSubsystem->AddMappingContext(*AbilityInputModeInputMappingContextMap.Find(InputMode), AbilityInputMappingContextPriority);
		}
		else
		{
			UE_LOG(LogCrys, Error, TEXT("The HeroPlayerController [%s] is missing an InputMappingContext for AbilityInputMode [%hhd]"), *GetNameSafe(GetOwner()), InputMode);
		}
	}
}

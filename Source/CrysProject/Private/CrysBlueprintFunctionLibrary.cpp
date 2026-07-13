// Copyright Soccertitan 2026


#include "CrysBlueprintFunctionLibrary.h"

#include "CrysLogChannels.h"
#include "Character/CrysCharacter.h"
#include "GameFramework/PlayerState.h"
#include "Player/CrysPlayerController.h"
#include "Player/CrysPlayerState.h"
#include "Settings/CrysGameData.h"
#include "System/CrysAssetManager.h"
#include "System/CrysGameplayTagRelationship.h"
#include "UI/CrysHUD.h"
#include "UI/ViewModel/CrysViewModel.h"

ACrysPlayerController* UCrysBlueprintFunctionLibrary::GetCrysPlayerController(AActor* Actor)
{
	if (const APawn* Pawn = Cast<APawn>(Actor))
	{
		return Cast<ACrysPlayerController>(Pawn->GetController());
	}

	if (const APlayerState* PlayerState = Cast<APlayerState>(Actor))
	{
		return Cast<ACrysPlayerController>(PlayerState->GetPlayerController());
	}
	
	return Cast<ACrysPlayerController>(Actor);
}

ACrysPlayerState* UCrysBlueprintFunctionLibrary::GetCrysPlayerState(AActor* Actor)
{
	ACrysPlayerState* Result = Cast<ACrysPlayerState>(Actor);
	
	if (Result)
	{
		return Result;
	}
	
	if (const APawn* Pawn = Cast<APawn>(Actor))
	{
		Result = Pawn->GetPlayerState<ACrysPlayerState>();
		if (Result)
		{
			return Result;
		}
	}

	if (const AController* Controller = Cast<AController>(Actor))
	{
		Result = Controller->GetPlayerState<ACrysPlayerState>();
		if (Result)
		{
			return Result;
		}
	}

	return Result;
}

ACrysCharacter* UCrysBlueprintFunctionLibrary::GetCrysCharacter(AActor* Actor)
{
	ACrysCharacter* Result = Cast<ACrysCharacter>(Actor);
	
	if (Result)
	{
		return Result;
	}
	
	if (const APlayerController* PC = Cast<APlayerController>(Actor))
	{
		return PC->GetPawn<ACrysCharacter>();
	}
	
	if (const APlayerState* PS = Cast<APlayerState>(Actor))
	{
		return PS->GetPawn<ACrysCharacter>();
	}
	
	return Result;
}

UCrysViewModel* UCrysBlueprintFunctionLibrary::FindOrCreateViewModel(const TSubclassOf<UCrysViewModel> ViewModelClass, APlayerController* PlayerController)
{
	if (IsValid(PlayerController) && ViewModelClass)
	{
		if (ACrysHUD* HUD = Cast<ACrysHUD>(PlayerController->GetHUD()))
		{
			return HUD->FindOrCreateViewModel(ViewModelClass);
		}
	}
	return nullptr;
}

const FCrysGameplayTagInfo* UCrysBlueprintFunctionLibrary::FindCrysGameplayTagInfo(const FGameplayTag& Tag, bool bLogNotFound)
{
	const UCrysGameplayTagRelationship* GameplayTagRelationship = UCrysAssetManager::GetAsset(GetDefault<UCrysGameData>()->GameplayTagRelationship, false);
	if (!GameplayTagRelationship)
	{
		if (bLogNotFound)
		{
			UE_LOG(LogCrys, Error, TEXT("GameplayTagRelationship is invalid in [%s]"), *GetDefault<UCrysGameData>()->GetName());
		}
		return nullptr;
	}

	return GameplayTagRelationship->FindInfo(Tag, bLogNotFound);
}

ETeamAttitude::Type UCrysBlueprintFunctionLibrary::GetAttitudeTowardsActor(AActor* Source, AActor* Target)
{
	const IGenericTeamAgentInterface* AgentInterface = Cast<const IGenericTeamAgentInterface>(Source);
	return AgentInterface ? AgentInterface->GetTeamAttitudeTowards(*Target) : ETeamAttitude::Neutral;
}

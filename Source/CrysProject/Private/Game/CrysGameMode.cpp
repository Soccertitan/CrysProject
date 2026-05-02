// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/CrysGameMode.h"

#include "Character/CrysCharacter.h"
#include "Game/CrysGameInstance.h"
#include "Game/CrysGameState.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "Player/CrysPlayerController.h"
#include "Player/CrysPlayerState.h"
#include "UI/CrysHUD.h"

ACrysGameMode::ACrysGameMode()
{
	bPauseable = false;
	bUseSeamlessTravel = true;

	GameStateClass = ACrysGameState::StaticClass();
	PlayerControllerClass = ACrysPlayerController::StaticClass();
	DefaultPawnClass = ACrysCharacter::StaticClass();
	PlayerStateClass = ACrysPlayerState::StaticClass();
	HUDClass = ACrysHUD::StaticClass();
}

UClass* ACrysGameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	// if (AHeroPlayerState* PlayerState = InController->GetPlayerState<AHeroPlayerState>())
	// {
	// 	//TODO: Get the HeroCharacterToSpawn.
	// 	// if (IsValid(PS->GetHeroCharacterToSpawn()))
	// 	// {
	// 	// 	return PS->GetHeroCharacterToSpawn();
	// 	// }
	// }
	
	return Super::GetDefaultPawnClassForController_Implementation(InController);
}

AActor* ACrysGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), Actors);
	AActor* SelectedActor = nullptr;

	FName PlayerStartTag = NAME_None;
	if (UCrysGameInstance* GameInstance = Cast<UCrysGameInstance>(GetGameInstance()))
	{
		PlayerStartTag = GameInstance->PlayerStartTag;
	}
	
	if (Actors.Num() > 0)
	{
		for (AActor* Actor : Actors)
		{
			if (APlayerStart* PlayerStart = Cast<APlayerStart>(Actor))
			{
				if (PlayerStart->PlayerStartTag == PlayerStartTag)
				{
					SelectedActor = PlayerStart;
					break;
				}	
			}
		}
	}

	if (IsValid(SelectedActor))
	{
		return SelectedActor;
	}
	
	return Super::ChoosePlayerStart_Implementation(Player);
}

void ACrysGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
}

FString ACrysGameMode::InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId,
	const FString& Options, const FString& Portal)
{
	//TODO: Old FFXI way to pass in the save data.
	// if (UGameplayStatics::HasOption(Options, FString("SaveGameData")))
	// {
	// 	FString GameProgressString = UGameplayStatics::ParseOption(Options, FString("SaveGameData"));
	// 	FGameProgressData GameProgressData;
	//
	// 	if (FJsonObjectConverter::JsonObjectStringToUStruct(GameProgressString, &GameProgressData))
	// 	{
	// 		if (ACrimsonPlayerState* PS = NewPlayerController->GetPlayerState<ACrimsonPlayerState>())
	// 		{
	// 			BindToSavingGameDelegates(PS);
	// 			PS->SetGameProgressData(GameProgressData);
	// 		}	
	// 	}
	// }
	
	return Super::InitNewPlayer(NewPlayerController, UniqueId, Options, Portal);
}

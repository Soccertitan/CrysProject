// Copyright Soccertitan 2026


#include "CrysBlueprintFunctionLibrary.h"

#include "CrysLogChannels.h"
#include "Character/CrysCharacter.h"
#include "GameFramework/PlayerState.h"
#include "Player/CrysPlayerController.h"
#include "Player/CrysPlayerState.h"
#include "Settings/CrysGameData.h"
#include "System/CrysAssetManager.h"

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

FAttributeTagInfo UCrysBlueprintFunctionLibrary::FindAttributeTagInfo(const FGameplayTag& Tag, bool bLogNotFound)
{
	const UAttributeTagRelationship* AttributeTagRelationship = UCrysAssetManager::GetAsset(GetDefault<UCrysGameData>()->AttributeTagRelationship);
	if (!AttributeTagRelationship)
	{
		if (bLogNotFound)
		{
			UE_LOG(LogCrys, Error, TEXT("AttributeTagRelationship is invalid in [%s]"), *GetDefault<UCrysGameData>()->GetName());
		}
		return FAttributeTagInfo();
	}

	return AttributeTagRelationship->FindAttributeTagInfo(Tag, bLogNotFound);
}

FUITagInfo UCrysBlueprintFunctionLibrary::FindUITagInfo(const FGameplayTag& Tag, bool bLogNotFound)
{
	const UUITagRelationship* UITagRelationship = UCrysAssetManager::GetAsset(GetDefault<UCrysGameData>()->UITagRelationship);
	if (!UITagRelationship)
	{
		if (bLogNotFound)
		{
			UE_LOG(LogCrys, Error, TEXT("UITagRelationship is invalid in [%s]"), *GetDefault<UCrysGameData>()->GetName());
		}
		return FUITagInfo();
	}

	return UITagRelationship->FindUITagInfo(Tag, bLogNotFound);
}

ETeamAttitude::Type UCrysBlueprintFunctionLibrary::GetAttitudeTowardsActor(AActor* Source, AActor* Target)
{
	const IGenericTeamAgentInterface* AgentInterface = Cast<const IGenericTeamAgentInterface>(Source);
	return AgentInterface ? AgentInterface->GetTeamAttitudeTowards(*Target) : ETeamAttitude::Neutral;
}

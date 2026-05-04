// Copyright Soccertitan 2026


#include "System/MapTransitionSubsystem.h"

#include "Game/CrysGameInstance.h"

void UMapTransitionSubsystem::StartMapTransition(TSoftObjectPtr<UWorld> InMap, FName InPlayerStartTag, int32 InTransitionDelay)
{
	if (bIsTransitioning || InMap.IsNull() || GetWorld()->GetNetMode() == NM_Client)
	{
		return;
	}

	bIsTransitioning = true;
	PlayerStartTag = InPlayerStartTag;
	TransitionDelay = InTransitionDelay;
	Map = InMap;
	StartMapTransitionTimer();
}

void UMapTransitionSubsystem::CancelMapTransition()
{
	if (!bIsTransitioning || GetWorld()->GetNetMode() == NM_Client)
	{
		return;
	}

	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_GoToMap);
	TimerHandle_GoToMap.Invalidate();
	
	bIsTransitioning = false;
	Multi_OnMapTransitionCancelled();
}

bool UMapTransitionSubsystem::DoesSupportWorldType(const EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::Game || WorldType == EWorldType::PIE;
}

void UMapTransitionSubsystem::TransitionToMap()
{
	if (UCrysGameInstance* GameInstance = Cast<UCrysGameInstance>(GetWorld()->GetGameInstance()))
	{
		GameInstance->PlayerStartTag = PlayerStartTag;
	}
	/** 
	 * TODO: Call a delegate to have systems respond to when a map transition is about to occur? Or leave it up to the 
	 * GameMode to call their internal systems other systems that respond. Probably makes the most sense since we other systems
	 * could trigger a transition outside of this Subsystem.
	 */

	FString URL = FPackageName::ObjectPathToPackageName(Map.ToString());
	GetWorld()->ServerTravel(URL);
}

void UMapTransitionSubsystem::StartMapTransitionTimer()
{
	Multi_OnMapTransitionStarted(Map, PlayerStartTag, TransitionDelay);

	if (TransitionDelay > 0)
	{
		GetWorld()->GetTimerManager().SetTimer(
			TimerHandle_GoToMap,
			this,
			&UMapTransitionSubsystem::TransitionToMap,
			TransitionDelay);
	}
	else
	{
		TransitionToMap();
	}
}

void UMapTransitionSubsystem::Multi_OnMapTransitionStarted_Implementation(const TSoftObjectPtr<UWorld>& InMap,
	const FName& InPlayerStartTag, const int32 InTransitionDelay)
{
	OnMapTransitionStartedDelegate.Broadcast(InMap, InPlayerStartTag, InTransitionDelay);
}

void UMapTransitionSubsystem::Multi_OnMapTransitionCancelled_Implementation()
{
	OnMapTransitionCancelledDelegate.Broadcast();
}

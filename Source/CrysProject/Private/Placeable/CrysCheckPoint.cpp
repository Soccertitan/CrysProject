// Copyright Soccertitan 2026


#include "Placeable/CrysCheckPoint.h"

#include "Game/CrysGameInstance.h"


ACrysCheckPoint::ACrysCheckPoint(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void ACrysCheckPoint::TriggerCheckPoint()
{
	if (!HasAuthority()) return;

	Multi_OnCheckPointTriggered();
	
	if (UCrysGameInstance* GameInstance = Cast<UCrysGameInstance>(GetGameInstance()))
	{
		GameInstance->PlayerStartTag = PlayerStartTag;
	}
}

void ACrysCheckPoint::Multi_OnCheckPointTriggered_Implementation()
{
	OnCheckPointTriggered();
}

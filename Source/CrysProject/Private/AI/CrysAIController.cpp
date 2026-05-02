// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/CrysAIController.h"


ACrysAIController::ACrysAIController()
{
	
}

void ACrysAIController::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
	if (IGenericTeamAgentInterface* Interface = Cast<IGenericTeamAgentInterface>(GetPawn()))
	{
		Interface->SetGenericTeamId(NewTeamID);
	}
}

FGenericTeamId ACrysAIController::GetGenericTeamId() const
{
	if (IGenericTeamAgentInterface* Interface = Cast<IGenericTeamAgentInterface>(GetPawn()))
	{
		return Interface->GetGenericTeamId();
	}
	return FGenericTeamId();
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ViewModel/CrysViewModel.h"

void UCrysViewModel::InitializeViewModel(APlayerController* PlayerController)
{
	if (IsValid(PlayerController))
	{
		K2_OnInitializeViewModel(PlayerController);
		
		PlayerController->OnPossessedPawnChanged.AddUniqueDynamic(this, &UCrysViewModel::OnPossessedPawnChanged);
		OnPossessedPawnChanged(nullptr, PlayerController->GetPawn());
	}
}

void UCrysViewModel::OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn)
{
	K2_OnPossessedPawnChanged(OldPawn, NewPawn);
}

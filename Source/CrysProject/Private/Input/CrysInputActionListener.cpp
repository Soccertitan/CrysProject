// Copyright Soccertitan 2026


#include "Input/CrysInputActionListener.h"

#include "InputActionValue.h"

void UCrysInputActionListener::OnInitializeListener()
{
	Super::OnInitializeListener();
	
	if (GetPlayerController())
	{
		GetPlayerController()->OnPossessedPawnChanged.AddUniqueDynamic(this, &UCrysInputActionListener::OnPossessedPawnChanged);
		OnPossessedPawnChanged(nullptr, GetPlayerController()->GetPawn());
	}
}

void UCrysInputActionListener::OnInputActionTriggered(const FInputActionValue& Value)
{
	Super::OnInputActionTriggered(Value);
	
	bPressed = Value.Get<bool>();
	if (bPressed)
	{
		OnInputPressed.Broadcast(this);
	}
	else
	{
		OnInputReleased.Broadcast(this);
	}
}

void UCrysInputActionListener::OnInputActionCanceled(const FInputActionValue& Value)
{
	Super::OnInputActionCanceled(Value);
	
	if (bPressed)
	{
		bPressed = false;
		OnInputReleased.Broadcast(this);
	}
}

void UCrysInputActionListener::OnInputActionCompleted(const FInputActionValue& Value)
{
	Super::OnInputActionCompleted(Value);
	
	if (bPressed)
	{
		bPressed = false;
		OnInputReleased.Broadcast(this);
	}
}

void UCrysInputActionListener::OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn)
{
	K2_OnPossesedPawnChanged(OldPawn, NewPawn);
}

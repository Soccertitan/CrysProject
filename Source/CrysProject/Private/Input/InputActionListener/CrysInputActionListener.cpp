// Copyright Soccertitan 2026


#include "Input/InputActionListener/CrysInputActionListener.h"

#include "InputActionValue.h"

void UCrysInputActionListener::Initialize()
{
	Super::Initialize();
	
	if (GetPlayerController())
	{
		GetPlayerController()->OnPossessedPawnChanged.AddUniqueDynamic(this, &UCrysInputActionListener::OnPossessedPawnChanged);
		OnPossessedPawnChanged(nullptr, GetPlayerController()->GetPawn());
	}
}

void UCrysInputActionListener::InputActionTriggered(const FInputActionValue& Value)
{
	Super::InputActionTriggered(Value);
	
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

void UCrysInputActionListener::InputActionCanceled(const FInputActionValue& Value)
{
	Super::InputActionCanceled(Value);
	
	if (bPressed)
	{
		bPressed = false;
		OnInputReleased.Broadcast(this);
	}
}

void UCrysInputActionListener::InputActionCompleted(const FInputActionValue& Value)
{
	Super::InputActionCompleted(Value);
	
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

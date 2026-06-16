// Copyright Soccertitan 2026


#include "Input/UI/InputActionListenerViewModel.h"

#include "Input/InputActionListener/CrysInputActionListener.h"


void UInputActionListenerViewModel::InitializeInputActionListener(UInputAction* InInputAction, UCrysInputActionListener* InInputActionListener)
{
	InputAction = InInputAction;
	SetInputActionListener(InInputActionListener);
}

void UInputActionListenerViewModel::SetInputActionListener(UCrysInputActionListener* InInputActionListener)
{
	if (InputActionListener)
	{
		InputActionListener->OnInputPressed.RemoveAll(this);
		InputActionListener->OnInputReleased.RemoveAll(this);
	}

	if (InInputActionListener)
	{
		InputActionListener = InInputActionListener;
		
		SetIsPressed(InputActionListener->IsPressed());
		InputActionListener->OnInputPressed.AddUniqueDynamic(this, &UInputActionListenerViewModel::OnInputPressed);
		InputActionListener->OnInputReleased.AddUniqueDynamic(this, &UInputActionListenerViewModel::OnInputReleased);
	}
}

void UInputActionListenerViewModel::SetIsPressed(bool InValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(bPressed, InValue);
}

void UInputActionListenerViewModel::OnInputPressed(UCrysInputActionListener* Action)
{
	SetIsPressed(true);
}

void UInputActionListenerViewModel::OnInputReleased(UCrysInputActionListener* Action)
{
	SetIsPressed(false);
}

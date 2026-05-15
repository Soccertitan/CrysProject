// Copyright Soccertitan 2026


#include "Input/InputActionListener/UI/InputActionViewModel.h"

#include "CrimEnhancedInputComponent.h"
#include "CrysLogChannels.h"
#include "Input/InputActionListener/CrysInputActionListener.h"
#include "Input/InputActionListener/UI/InputActionListenerViewModel.h"


void UInputActionViewModel::InitializeViewModel(APlayerController* PlayerController)
{
	Super::InitializeViewModel(PlayerController);
	
	InitEnhancedInputComponent(PlayerController);
}

UInputActionListenerViewModel* UInputActionViewModel::FindOrCreateInputActionListenerViewModel(UInputAction* InputAction)
{
	if (InputAction)
	{
		for (UInputActionListenerViewModel* ViewModel : InputActionListenerViewModels)
		{
			if (ViewModel->GetInputAction() == InputAction)
			{
				return ViewModel;
			}
		}
		return InternalCreateInputActionListenerViewModel(InputAction);
	}
	return nullptr;
}

void UInputActionViewModel::InitEnhancedInputComponent(APlayerController* PlayerController)
{
	if (EnhancedInputComponent)
	{
		return;
	}
	
	EnhancedInputComponent = Cast<UCrimEnhancedInputComponent>(PlayerController->InputComponent);
	if (!EnhancedInputComponent)
	{
		UE_LOG(LogCrys, Error, TEXT("%s does not have a CrimEnhancedInputComponent"), *GetNameSafe(PlayerController));
		return;
	}
}

UInputActionListenerViewModel* UInputActionViewModel::InternalCreateInputActionListenerViewModel(UInputAction* InputAction)
{
	UInputActionListenerViewModel* NewVM = NewObject<UInputActionListenerViewModel>(this);
	UCrysInputActionListener* InputActionListener = nullptr;
	if (EnhancedInputComponent)
	{
		InputActionListener = Cast<UCrysInputActionListener>(EnhancedInputComponent->FindListener(InputAction));
	}
	NewVM->InitializeInputActionListener(InputAction, InputActionListener);
	InputActionListenerViewModels.Add(NewVM);
	return NewVM;
}

// Copyright Soccertitan 2026


#include "Input/InputActionListener/UI/InputActionManagerViewModel.h"

#include "CrimEnhancedInputComponent.h"
#include "CrysLogChannels.h"
#include "Input/InputActionListener/CrysInputActionListener.h"
#include "Input/InputActionListener/UI/InputActionListenerViewModel.h"


void UInputActionManagerViewModel::InitializeViewModel(APlayerController* PlayerController)
{
	Super::InitializeViewModel(PlayerController);
	
	InitEnhancedInputComponent(PlayerController);
}

UInputActionListenerViewModel* UInputActionManagerViewModel::FindOrCreateInputActionListenerViewModel(UInputAction* InputAction)
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
		return CreateInputActionListenerViewModelInternal(InputAction);
	}
	return nullptr;
}

void UInputActionManagerViewModel::InitEnhancedInputComponent(APlayerController* PlayerController)
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
	
	EnhancedInputComponent->OnListenerAdded.AddUniqueDynamic(this, &UInputActionManagerViewModel::OnInputActionListenerAdded);
	EnhancedInputComponent->OnListenerRemoved.AddUniqueDynamic(this, &UInputActionManagerViewModel::OnInputActionListenerRemoved);
	for (const FCrimEnhancedInputActionBinding& Binding : EnhancedInputComponent->GetListeners())
	{
		OnInputActionListenerAdded(Binding);
	}
}

UInputActionListenerViewModel* UInputActionManagerViewModel::CreateInputActionListenerViewModelInternal(UInputAction* InputAction)
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

void UInputActionManagerViewModel::OnInputActionListenerAdded(const FCrimEnhancedInputActionBinding& InputActionBinding)
{
	for (UInputActionListenerViewModel* ViewModel : InputActionListenerViewModels)
	{
		if (InputActionBinding.GetInputAction() == ViewModel->GetInputAction())
		{
			ViewModel->SetInputActionListener(Cast<UCrysInputActionListener>(InputActionBinding.GetListener()));
		}
	}
}

void UInputActionManagerViewModel::OnInputActionListenerRemoved(const FCrimEnhancedInputActionBinding& InputActionBinding)
{
	for (UInputActionListenerViewModel* ViewModel : InputActionListenerViewModels)
	{
		if (InputActionBinding.GetInputAction() == ViewModel->GetInputAction())
		{
			ViewModel->SetInputActionListener(nullptr);
		}
	}
}

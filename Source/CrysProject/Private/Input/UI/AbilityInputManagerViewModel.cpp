// Copyright Soccertitan 2026


#include "Input/UI/AbilityInputManagerViewModel.h"

#include "CrimAbilitySystemBlueprintFunctionLibrary.h"
#include "CrimAbilitySystemComponent.h"
#include "CrysLogChannels.h"
#include "Abilities/GameplayAbility.h"
#include "AbilitySystem/UI/AbilityViewModel.h"
#include "Input/UI/AbilityInputSlotViewModel.h"
#include "Input/AbilityInputManagerComponent.h"
#include "Player/CrysPlayerState.h"
#include "Player/HeroPlayerController.h"

void UAbilityInputManagerViewModel::InitializeViewModel(APlayerController* PlayerController)
{
	Super::InitializeViewModel(PlayerController);
	
	InitAbilityInputManager(PlayerController);
	
	HeroPlayerController = Cast<AHeroPlayerController>(PlayerController);
	check(HeroPlayerController);
	SetInputMode(HeroPlayerController->GetAbilityInputMode());
	HeroPlayerController->OnAbilityInputModeChangedDelegate.AddUniqueDynamic(this, &UAbilityInputManagerViewModel::SetInputMode);
	
	AbilitySystemComponent = UCrimAbilitySystemBlueprintFunctionLibrary::GetAbilitySystemComponent(PlayerController->GetPlayerState<ACrysPlayerState>());
}

UAbilityInputSlotViewModel* UAbilityInputManagerViewModel::FindOrCreateInputSlotViewModel(const FAbilityInputSlot& InputSlot, const int32 InputSet)
{
	UAbilityInputSlotViewModel* Result = FindInputSlotViewModel(InputSlot, InputSet);
	if (!Result)
	{
		Result = NewObject<UAbilityInputSlotViewModel>(this);
		Result->InputSlot = InputSlot;
		Result->InputSet = InputSet;
		
		bool bFoundInputSetViewModel = false;
		for (FAbilityInputSetViewModel& InputSetViewModel : AbilityInputSetViewModels)
		{
			if (InputSetViewModel.InputSet == InputSet)
			{
				bFoundInputSetViewModel = true;
				InputSetViewModel.ViewModels.Add(Result);
			}
		}
		if (!bFoundInputSetViewModel)
		{
			FAbilityInputSetViewModel& InputSetViewModel = AbilityInputSetViewModels.AddDefaulted_GetRef();
			InputSetViewModel.InputSet = InputSet;
			InputSetViewModel.ViewModels.Add(Result);
		}
		
		if (AbilityInputManagerComponent)
		{
			FAbilityInputInstance InputInstance = AbilityInputManagerComponent->FindAbilityInputInstance(InputSlot, InputSet);
			Result->SetAbilityViewModel(CreateAbilityViewModel(InputInstance));
		}
		else
		{
			Result->SetAbilityViewModel(EmptyAbilityViewModel);
		}
	}
	return Result;
}

void UAbilityInputManagerViewModel::SetInputSlotAbility(UAbilityInputSlotViewModel* InputSlotViewModel, const TSubclassOf<UGameplayAbility> AbilityClass)
{
	if (AbilityInputManagerComponent && InputSlotViewModel)
	{
		if (AbilityClass)
		{
			FAbilityInputParams Params;
			Params.Slot = InputSlotViewModel->GetInputSlot();
			Params.AbilityClass = AbilityClass;
			AbilityInputManagerComponent->SetAbilityInput(Params, InputSlotViewModel->InputSet);
		}
		else
		{
			AbilityInputManagerComponent->ClearAbilityInput(InputSlotViewModel->GetInputSlot(), InputSlotViewModel->InputSet);
		}
	}
}

void UAbilityInputManagerViewModel::SetActiveInputSet(const int32 InValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(ActiveInputSet, InValue);
}

void UAbilityInputManagerViewModel::SetInputMode(const EAbilityInputMode InValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(InputMode, InValue);
}

UAbilityInputSlotViewModel* UAbilityInputManagerViewModel::FindInputSlotViewModel(const FAbilityInputSlot& InputSlot, const int32 InputSet)
{
	for (const FAbilityInputSetViewModel& InputSetViewModel : AbilityInputSetViewModels)
	{
		if (InputSetViewModel.InputSet == InputSet)
		{
			for (UAbilityInputSlotViewModel* ViewModel : InputSetViewModel.ViewModels)
			{
				if (ViewModel->GetInputSlot() == InputSlot)
				{
					return ViewModel;
				}
			}
			break;
		}
	}
	return nullptr;
}

void UAbilityInputManagerViewModel::InitAbilityInputManager(APlayerController* PlayerController)
{
	if (AbilityInputManagerComponent)
	{
		return;
	}
	
	AbilityInputManagerComponent = PlayerController->FindComponentByClass<UAbilityInputManagerComponent>();
	if (!AbilityInputManagerComponent)
	{
		UE_LOG(LogCrys, Error, TEXT("%s does not have an AbilityInputManagerComponent"), *GetNameSafe(PlayerController));
	}
	
	SetActiveInputSet(AbilityInputManagerComponent->GetActiveInputSet());
	AbilityInputManagerComponent->OnInputSetChangedDelegate.AddUniqueDynamic(this, &UAbilityInputManagerViewModel::SetActiveInputSet);
	AbilityInputManagerComponent->OnAbilityInputAddedDelegate.AddUniqueDynamic(this, &UAbilityInputManagerViewModel::OnAbilityInputAdded);
	AbilityInputManagerComponent->OnAbilityInputChangedDelegate.AddUniqueDynamic(this, &UAbilityInputManagerViewModel::OnAbilityInputChanged);
	AbilityInputManagerComponent->OnAbilityInputRemovedDelegate.AddUniqueDynamic(this, &UAbilityInputManagerViewModel::OnAbilityInputRemoved);
	
	EmptyAbilityViewModel = NewObject<UAbilityViewModel>(this);
}

void UAbilityInputManagerViewModel::OnAbilityInputAdded(const FAbilityInputInstance& AbilityInputInstance, const int32 InputSet)
{
	UAbilityInputSlotViewModel* ViewModel = FindInputSlotViewModel(AbilityInputInstance.InputSlot, InputSet);
	if (ViewModel)
	{
		ViewModel->SetAbilityViewModel(CreateAbilityViewModel(AbilityInputInstance));
	}
}

void UAbilityInputManagerViewModel::OnAbilityInputChanged(const FAbilityInputInstance& AbilityInputInstance, const int32 InputSet)
{
	UAbilityInputSlotViewModel* ViewModel = FindInputSlotViewModel(AbilityInputInstance.InputSlot, InputSet);
	if (ViewModel)
	{
		ViewModel->SetAbilityViewModel(CreateAbilityViewModel(AbilityInputInstance));
	}
}

void UAbilityInputManagerViewModel::OnAbilityInputRemoved(const FAbilityInputInstance& AbilityInputInstance, const int32 InputSet)
{
	UAbilityInputSlotViewModel* ViewModel = FindInputSlotViewModel(AbilityInputInstance.InputSlot, InputSet);
	if (ViewModel)
	{
		ViewModel->SetAbilityViewModel(EmptyAbilityViewModel);
	}
}

UAbilityViewModel* UAbilityInputManagerViewModel::CreateAbilityViewModel(const FAbilityInputInstance& AbilityInputInstance)
{
	if (AbilityInputInstance.AbilityClass)
	{
		UAbilityViewModel* NewVM = NewObject<UAbilityViewModel>(this);
		NewVM->SetGameplayAbility(AbilityInputInstance.AbilityClass, AbilitySystemComponent);
		return NewVM;
	}
	return EmptyAbilityViewModel;
}

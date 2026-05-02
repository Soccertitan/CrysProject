// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CrysHUD.h"

#include "Blueprint/UserWidget.h"
#include "UI/ViewModel/CrysViewModel.h"

void ACrysHUD::CreateHUDWidget()
{
	if (HUDWidgetClass)
	{
		if (HUDWidget)
		{
			HUDWidget->RemoveFromParent();
		}
	
		HUDWidget = CreateWidget<UUserWidget>(GetOwningPlayerController(), HUDWidgetClass);
		HUDWidget->AddToPlayerScreen();
	}
}

UCrysViewModel* ACrysHUD::FindOrCreateViewModel(const TSubclassOf<UCrysViewModel> ViewModelClass)
{
	ViewModels.Remove(nullptr);
	
	if (!ViewModelClass)
	{
		return nullptr;
	}

	for (const TWeakObjectPtr<UCrysViewModel>& VM : ViewModels)
	{
		if (IsValid(VM.Get()) && VM->IsA(ViewModelClass))
		{
			return VM.Get();
		}
	}

	UCrysViewModel* ViewModel = NewObject<UCrysViewModel>(this, ViewModelClass);
	ViewModel->InitializeViewModel(GetOwningPlayerController());
	ViewModels.Add(ViewModel);
	return ViewModel;
}

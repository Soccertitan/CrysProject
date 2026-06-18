// Copyright Soccertitan 2026


#include "UI/ViewModel/SettingsViewModel.h"

#include "Player/HeroPlayerController.h"

void USettingsViewModel::InitializeViewModel(APlayerController* PlayerController)
{
	Super::InitializeViewModel(PlayerController);
	OwnerPC = PlayerController;
	
	if (AHeroPlayerController* HeroPC = Cast<AHeroPlayerController>(PlayerController))
	{
		SetAbilityInputMode(HeroPC->GetAbilityInputMode());
		HeroPC->OnAbilityInputModeChangedDelegate.AddUniqueDynamic(this, &USettingsViewModel::SetAbilityInputMode);
	}
}

void USettingsViewModel::K2_SetAbilityInputMode(const EAbilityInputMode NewValue)
{
	if (AHeroPlayerController* HeroPC = Cast<AHeroPlayerController>(OwnerPC))
	{
		HeroPC->SetAbilityInputMode(NewValue);
	}
}

void USettingsViewModel::SetAbilityInputMode(const EAbilityInputMode NewValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(AbilityInputMode, NewValue);
}

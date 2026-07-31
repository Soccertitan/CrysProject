// Copyright Soccertitan 2026


#include "AbilitySystem/UI/PlayerActivatableAbilitiesViewModel.h"

#include "CrimAbilitySystemBlueprintFunctionLibrary.h"
#include "AbilitySystem/UI/ActivatableAbilitiesViewModel.h"
#include "Player/CrysPlayerState.h"

void UPlayerActivatableAbilitiesViewModel::InitializeViewModel(APlayerController* PlayerController)
{
	AbilitySystemComponent = UCrimAbilitySystemBlueprintFunctionLibrary::GetAbilitySystemComponent(PlayerController->GetPlayerState<ACrysPlayerState>());
	
	ActivatableAbilitiesViewModel = NewObject<UActivatableAbilitiesViewModel>(this);
	ActivatableAbilitiesViewModel->SetAbilitySystemComponent(AbilitySystemComponent);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(ActivatableAbilitiesViewModel);
	
	Super::InitializeViewModel(PlayerController);
}

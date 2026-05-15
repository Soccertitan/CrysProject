// Copyright Soccertitan 2026


#include "AbilitySystem/UI/PlayerAttributesViewModel.h"

#include "CrimAbilitySystemBlueprintFunctionLibrary.h"
#include "Player/CrysPlayerState.h"

void UPlayerAttributesViewModel::InitializeViewModel(APlayerController* PlayerController)
{
	Super::InitializeViewModel(PlayerController);
	
	if (UCrimAbilitySystemComponent* AbilitySystemComponent = UCrimAbilitySystemBlueprintFunctionLibrary::GetAbilitySystemComponent(PlayerController->GetPlayerState<ACrysPlayerState>()))
	{
		OnAbilitySystemSet(AbilitySystemComponent);
	}
}

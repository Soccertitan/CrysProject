// Copyright Soccertitan 2026


#include "UI/ViewModel/HeroOverlayViewModel.h"

#include "CrimAbilitySystemBlueprintFunctionLibrary.h"
#include "CrysBlueprintFunctionLibrary.h"
#include "AbilitySystem/Ability/Combat/CombatSystemComponent.h"
#include "Player/CrysPlayerController.h"
#include "Player/CrysPlayerState.h"

void UHeroOverlayViewModel::InitializeViewModel(APlayerController* PlayerController)
{
	Super::InitializeViewModel(PlayerController);
	
	if (UCrimAbilitySystemComponent* AbilitySystemComponent = UCrimAbilitySystemBlueprintFunctionLibrary::GetAbilitySystemComponent(PlayerController->GetPlayerState<ACrysPlayerState>()))
	{
		OnAbilitySystemComponentRetrieved(AbilitySystemComponent);
	}
	
	if (ACrysPlayerController* CrysPC = Cast<ACrysPlayerController>(PlayerController))
	{
		CrysPC->OnRootWidgetAddedDelegate.AddUObject(this, &UHeroOverlayViewModel::OnRootWidgetAdded);
		CrysPC->OnRootWidgetRemovedDelegate.AddUObject(this, &UHeroOverlayViewModel::OnRootWidgetRemoved);
	}
	
	CombatSystemComponent = UCrysBlueprintFunctionLibrary::GetCombatSystemComponent(PlayerController->GetPlayerState<APlayerState>());
	if (CombatSystemComponent)
	{
		SetIsAutoAttacking(CombatSystemComponent->IsAutoAttacking());
		CombatSystemComponent->OnAutoAttackStateChangedDelegate.AddUniqueDynamic(this, &UHeroOverlayViewModel::SetIsAutoAttacking);
		OnPrimaryTargetChanged(CombatSystemComponent->GetPrimaryTarget());
		CombatSystemComponent->OnPrimaryTargetChangedDelegate.AddUniqueDynamic(this, &UHeroOverlayViewModel::OnPrimaryTargetChanged);
	}
}

void UHeroOverlayViewModel::SetIsAutoAttacking(bool Value)
{
	UE_MVVM_SET_PROPERTY_VALUE(bAutoAttacking, Value);
}

void UHeroOverlayViewModel::OnRootWidgetAdded()
{
	SetIsRootWidgetAdded(true);
}

void UHeroOverlayViewModel::OnRootWidgetRemoved()
{
	SetIsRootWidgetAdded(false);
}

void UHeroOverlayViewModel::SetIsRootWidgetAdded(bool Value)
{
	UE_MVVM_SET_PROPERTY_VALUE(bRootWidgetAdded, Value);
}

void UHeroOverlayViewModel::OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn)
{
	Super::OnPossessedPawnChanged(OldPawn, NewPawn);
}

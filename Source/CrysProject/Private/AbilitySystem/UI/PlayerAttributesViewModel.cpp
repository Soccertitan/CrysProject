// Copyright Soccertitan 2026


#include "AbilitySystem/UI/PlayerAttributesViewModel.h"

#include "CrimAbilitySystemBlueprintFunctionLibrary.h"
#include "CrimAbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/UI/AttributeFractionViewModel.h"
#include "AbilitySystem/UI/AttributeViewModel.h"
#include "GameFramework/PlayerController.h"
#include "Player/CrysPlayerState.h"

void UPlayerAttributesViewModel::InitializeViewModel(APlayerController* PlayerController)
{
	AbilitySystemComponent = UCrimAbilitySystemBlueprintFunctionLibrary::GetAbilitySystemComponent(PlayerController->GetPlayerState<ACrysPlayerState>());
	
	Super::InitializeViewModel(PlayerController);
}

UAttributeViewModel* UPlayerAttributesViewModel::FindOrCreateAttributeViewModel(const FGameplayTag& AttributeTag)
{
	if (AttributeTag.IsValid())
	{
		for (TWeakObjectPtr<UAttributeViewModel> WeakViewModel : AttributeViewModels)
		{
			if (UAttributeViewModel* ViewModel = WeakViewModel.Get())
			{
				if (ViewModel->GetAttributeTag() == AttributeTag)
				{
					return ViewModel;
				}
			}
		}
		UAttributeViewModel* NewViewModel = NewObject<UAttributeViewModel>();
		NewViewModel->SetAttributeWithASC(AttributeTag, AbilitySystemComponent);
		AttributeViewModels.Add(NewViewModel);
		return NewViewModel;
	}
	return nullptr;
}

UAttributeFractionViewModel* UPlayerAttributesViewModel::FindOrCreateAttributeFractionViewModel(
	const FGameplayTag& NumeratorAttributeTag, const FGameplayTag DenominatorAttributeTag)
{
	if (NumeratorAttributeTag.IsValid() && DenominatorAttributeTag.IsValid())
	{
		for (TWeakObjectPtr<UAttributeFractionViewModel> WeakViewModel : AttributeFractionViewModels)
		{
			if (UAttributeFractionViewModel* ViewModel = WeakViewModel.Get())
			{
				if (ViewModel->GetNumeratorAttribute()->GetAttributeTag() == NumeratorAttributeTag &&
					ViewModel->GetDenominatorAttribute()->GetAttributeTag() == DenominatorAttributeTag)
				{
					return ViewModel;
				}
			}
		}
		UAttributeFractionViewModel* NewViewModel = NewObject<UAttributeFractionViewModel>();
		NewViewModel->SetAttributesWithASC(NumeratorAttributeTag, DenominatorAttributeTag, AbilitySystemComponent);
		AttributeFractionViewModels.Add(NewViewModel);
		return NewViewModel;
	}
	return nullptr;
}

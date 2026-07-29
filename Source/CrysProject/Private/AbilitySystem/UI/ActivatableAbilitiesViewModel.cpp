// Copyright Soccertitan 2026


#include "AbilitySystem/UI/ActivatableAbilitiesViewModel.h"

#include "CrimAbilitySystemComponent.h"
#include "AbilitySystem/UI/AbilityViewModel.h"

void UActivatableAbilitiesViewModel::SetAbilitySystemComponent(UCrimAbilitySystemComponent* ASC)
{
	if (ASC != AbilitySystemComponent)
	{
		if (AbilitySystemComponent)
		{
			AbilitySystemComponent->OnAbilityGivenDelegate.RemoveAll(this);
			AbilitySystemComponent->OnAbilityRemovedDelegate.RemoveAll(this);
		}
		
		AbilitySystemComponent = ASC;
		AbilityViewModels.Empty();
		if (AbilitySystemComponent)
		{
			AbilitySystemComponent->OnAbilityGivenDelegate.AddUObject(this, &UActivatableAbilitiesViewModel::OnAbilityGiven);
			AbilitySystemComponent->OnAbilityRemovedDelegate.AddUObject(this, &UActivatableAbilitiesViewModel::OnAbilityRemoved);
			
			AbilityViewModels.Reserve(AbilitySystemComponent->GetActivatableAbilities().Num());
			for (const FGameplayAbilitySpec& Spec : AbilitySystemComponent->GetActivatableAbilities())
			{
				UAbilityViewModel* NewVM = NewObject<UAbilityViewModel>(this);
				NewVM->SetGameplayAbility(Spec.Ability);
				AbilityViewModels.Add(NewVM);
			}
		}
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetAbilityViewModels);
	}
}

void UActivatableAbilitiesViewModel::OnAbilityGiven(const FGameplayAbilitySpec& Spec)
{
	for (UAbilityViewModel* ViewModel : AbilityViewModels)
	{
		if (ViewModel->GetGameplayAbilityClass() == Spec.Ability->GetClass())
		{
			// Found an existing ability that matches return early.
			return;
		}
	}
	
	// Did not find an existing view model so we create one now.
	UAbilityViewModel* NewVM = NewObject<UAbilityViewModel>(this);
	NewVM->SetGameplayAbility(Spec.Ability);
	AbilityViewModels.Add(NewVM);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetAbilityViewModels);
}

void UActivatableAbilitiesViewModel::OnAbilityRemoved(const FGameplayAbilitySpec& Spec)
{
	for (int32 Index = AbilityViewModels.Num(); Index >= 0; --Index)
	{
		if (AbilityViewModels[Index]->GetGameplayAbilityClass() == Spec.Ability->GetClass())
		{
			AbilityViewModels.RemoveAt(Index);
			break;
		}
	}
	
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetAbilityViewModels);
}

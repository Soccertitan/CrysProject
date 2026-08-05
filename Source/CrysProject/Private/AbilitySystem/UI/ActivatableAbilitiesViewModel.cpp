// Copyright Soccertitan 2026


#include "AbilitySystem/UI/ActivatableAbilitiesViewModel.h"

#include "CrimAbilitySystemComponent.h"
#include "CrysNativeGameplayTags.h"
#include "AbilitySystem/Ability/CrysGameplayAbility.h"
#include "AbilitySystem/UI/CrysAbilityViewModel.h"

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
				TryCreateViewModel(Spec);
			}
		}
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetAbilityViewModels);
	}
}

void UActivatableAbilitiesViewModel::OnAbilityGiven(const FGameplayAbilitySpec& Spec)
{
	for (UCrysAbilityViewModel* ViewModel : AbilityViewModels)
	{
		if (ViewModel->GetGameplayAbilityClass() == Spec.Ability->GetClass())
		{
			// Found an existing ability that matches return early.
			return;
		}
	}
	
	if (TryCreateViewModel(Spec))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetAbilityViewModels);
	}
}

void UActivatableAbilitiesViewModel::OnAbilityRemoved(const FGameplayAbilitySpec& Spec)
{
	for (int32 Index = AbilityViewModels.Num() - 1; Index >= 0; Index--)
	{
		if (AbilityViewModels[Index]->GetGameplayAbilityClass() == Spec.Ability->GetClass())
		{
			AbilityViewModels.RemoveAt(Index);
			break;
		}
	}
	
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetAbilityViewModels);
}

bool UActivatableAbilitiesViewModel::TryCreateViewModel(const FGameplayAbilitySpec& Spec)
{
	if (Spec.Ability->IsA(UCrysGameplayAbility::StaticClass()))
	{
		if (!Spec.Ability->GetAssetTags().HasTag(Crys::NativeGameplayTag::Abillity_Exclude_ViewModel))
		{
			UCrysAbilityViewModel* NewVM = NewObject<UCrysAbilityViewModel>(this);
			NewVM->SetGameplayAbility(Spec, AbilitySystemComponent);
			AbilityViewModels.Add(NewVM);
			return true;
		}
	}
	return false;
}

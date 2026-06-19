// Copyright Soccertitan 2026


#include "AbilitySystem/UI/AbilityViewModel.h"

#include "CrimAbilitySystemComponent.h"
#include "AbilitySystem/GameplayAbilityData.h"

float UAbilityViewModel::GetCooldownTimeRemaining() const
{
	if (IsAbilityOnCooldown() && GetAbilitySystemComponent())
	{
		FGameplayEffectQuery GameplayEffectQuery = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(CooldownTags);
        TArray<float> TimesRemaining = GetAbilitySystemComponent()->GetActiveEffectsTimeRemaining(GameplayEffectQuery);
        float TimeRemaining = 0.f;
        if (TimesRemaining.Num() > 0)
        {
        	TimeRemaining = TimesRemaining[0];
        	for (int32 i = 0; i < TimesRemaining.Num(); i++)
        	{
        		if (TimesRemaining[i] > TimeRemaining)
        		{
        			TimeRemaining = TimesRemaining[i];
        		}
        	}
        }
		return TimeRemaining;
	}
	return 0.f;
}

void UAbilityViewModel::SetAbilitySystemComponent(UCrimAbilitySystemComponent* NewAbilitySystemComponent)
{
	if (AbilitySystemComponent != NewAbilitySystemComponent)
	{
		UCrimAbilitySystemComponent* OldAbilitySystemComponent = AbilitySystemComponent;
		AbilitySystemComponent = NewAbilitySystemComponent;
		OnAbilitySystemComponentSet(OldAbilitySystemComponent);
		if (GameplayAbilityData)
		{
			UpdateViewModelData(GameplayAbilityData);
		}
	}
}

void UAbilityViewModel::SetGameplayAbilityData(UGameplayAbilityData* AbilityData)
{
	if (GameplayAbilityData != AbilityData)
	{
		GameplayAbilityData = AbilityData;
		if (GameplayAbilityData)
		{
			UpdateViewModelData(GameplayAbilityData);
		}
	}
}

void UAbilityViewModel::SetAbilityName(const FText& NewValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(AbilityName, NewValue);
}

void UAbilityViewModel::SetIcon(const TSoftObjectPtr<UTexture2D>& NewValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(Icon, NewValue);
}

void UAbilityViewModel::SetIsAbilityGranted(const bool NewValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(bAbilityGranted, NewValue);
}

void UAbilityViewModel::SetIsAbilityOnCooldown(const bool NewValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(bAbilityOnCooldown, NewValue);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetCooldownTimeRemaining);
}

void UAbilityViewModel::UpdateViewModelData(UGameplayAbilityData* AbilityData)
{
	SetAbilityName(AbilityData->AbilityName);
	SetIcon(AbilityData->Icon);
	UpdateIsAbilityGranted();
}

void UAbilityViewModel::OnAbilitySystemComponentSet(UCrimAbilitySystemComponent* OldAbilitySystemComponent)
{
	if (OldAbilitySystemComponent)
	{
		OldAbilitySystemComponent->OnAbilityGivenDelegate.RemoveAll(this);
		OldAbilitySystemComponent->OnAbilityRemovedDelegate.RemoveAll(this);
		UnbindToAbilityCooldownTags(OldAbilitySystemComponent);
	}
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->OnAbilityGivenDelegate.AddUObject(this, &UAbilityViewModel::OnAbilityGiven);
		AbilitySystemComponent->OnAbilityRemovedDelegate.AddUObject(this, &UAbilityViewModel::OnAbilityRemoved);
	}
}

void UAbilityViewModel::OnAbilityGiven(const FGameplayAbilitySpec& Spec)
{
	if (GetGameplayAbilityData())
	{
		if (GetGameplayAbilityData()->AbilityClass.Get() == Spec.Ability->GetClass())
		{
			SetIsAbilityGranted(true);
			BindToAbilityCooldownTags(Spec);
		}
	}
}

void UAbilityViewModel::OnAbilityRemoved(const FGameplayAbilitySpec& Spec)
{
	if (GetGameplayAbilityData())
	{
		if (GetGameplayAbilityData()->AbilityClass.Get() == Spec.Ability->GetClass())
		{
			SetIsAbilityGranted(false);
			UnbindToAbilityCooldownTags(AbilitySystemComponent);
		}
	}
}

FGameplayTagContainer UAbilityViewModel::GenerateCooldownTags(UGameplayAbility* GameplayAbility) const
{
	return *GameplayAbility->GetCooldownTags();
}

void UAbilityViewModel::UpdateIsAbilityGranted()
{
	if (AbilitySystemComponent)
	{
		FGameplayAbilitySpec* AbilitySpec = AbilitySystemComponent->FindAbilitySpecFromClass(GetGameplayAbilityData()->AbilityClass.Get());
		if (AbilitySpec)
		{
			OnAbilityGiven(*AbilitySpec);
		}
	}
	SetIsAbilityGranted(false);
}

void UAbilityViewModel::BindToAbilityCooldownTags(const FGameplayAbilitySpec& Spec)
{
	CooldownTags = GenerateCooldownTags(Spec.Ability);
	
	if (CooldownTags.Num() > 0)
	{
		for (const FGameplayTag& Tag : CooldownTags)
		{
			HandleCooldownTagCountChanged(Tag, AbilitySystemComponent->GetGameplayTagCount(Tag));
			BoundCooldownTagsASCHandles.Add(Tag, AbilitySystemComponent->RegisterGameplayTagEvent(Tag, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UAbilityViewModel::HandleCooldownTagCountChanged));
		}
	}
}

void UAbilityViewModel::UnbindToAbilityCooldownTags(UCrimAbilitySystemComponent* ASC)
{
	for (auto& Elem : BoundCooldownTagsASCHandles)
	{
		ASC->RegisterGameplayTagEvent(Elem.Key, EGameplayTagEventType::NewOrRemoved).Remove(Elem.Value);
	}
}

void UAbilityViewModel::HandleCooldownTagCountChanged(const FGameplayTag GameplayTag, int32 Count)
{
	//Cooldown applied
	if (Count > 0)
	{
		SetIsAbilityOnCooldown(true);
	}
	else
	{
		SetIsAbilityOnCooldown(false);
	}
}

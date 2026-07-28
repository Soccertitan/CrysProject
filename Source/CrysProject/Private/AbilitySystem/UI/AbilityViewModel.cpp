// Copyright Soccertitan 2026


#include "AbilitySystem/UI/AbilityViewModel.h"

#include "CrimAbilitySystemComponent.h"
#include "AbilitySystem/Ability/CrysGameplayAbility.h"


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

void UAbilityViewModel::SetGameplayAbility(const FGameplayAbilitySpec& Spec, UCrimAbilitySystemComponent* NewAbilitySystemComponent)
{
	SetAbilitySystemComponent(NewAbilitySystemComponent);
	UGameplayAbility* NewAbility = Spec.GetPrimaryInstance();
	if (!NewAbility)
	{
		NewAbility = Spec.Ability;
	}
	SetIsAbilityGranted(NewAbility ? true : false);
	SetGameplayAbility(NewAbility);
}

void UAbilityViewModel::SetGameplayAbility(TSubclassOf<UGameplayAbility> InAbilityClass, UCrimAbilitySystemComponent* NewAbilitySystemComponent)
{
	SetAbilitySystemComponent(NewAbilitySystemComponent);
	
	UGameplayAbility* NewAbility = nullptr;
	if (InAbilityClass)
	{
		if (NewAbilitySystemComponent)
		{
			FGameplayAbilitySpec* Spec = NewAbilitySystemComponent->FindAbilitySpecFromClass(InAbilityClass);
			if (Spec)
			{
				SetGameplayAbility(*Spec, NewAbilitySystemComponent);
				return;
			}
		}
		NewAbility = NewObject<UGameplayAbility>(this, InAbilityClass);
	}
	SetIsAbilityGranted(false);
	SetGameplayAbility(NewAbility);
}

void UAbilityViewModel::SetGameplayAbility(UGameplayAbility* InAbility)
{
	if (Ability != InAbility)
	{
		UGameplayAbility* OldAbility = Ability;
		Ability = InAbility;
		AbilityClass = Ability ? Ability->GetClass() : nullptr;
		TryBindToAbilityCooldownTags();
		OnGameplayAbilitySet(Ability, OldAbility);
	}
}

void UAbilityViewModel::SetAbilitySystemComponent(UCrimAbilitySystemComponent* NewAbilitySystemComponent)
{
	if (AbilitySystemComponent != NewAbilitySystemComponent)
	{
		UCrimAbilitySystemComponent* OldAbilitySystemComponent = AbilitySystemComponent;
		AbilitySystemComponent = NewAbilitySystemComponent;
		OnAbilitySystemComponentSet(AbilitySystemComponent, OldAbilitySystemComponent);
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

void UAbilityViewModel::OnGameplayAbilitySet(UGameplayAbility* NewAbility, UGameplayAbility* OldAbility)
{
	UCrysGameplayAbility* CrysGameplayAbility = Cast<UCrysGameplayAbility>(NewAbility);
	SetAbilityName(CrysGameplayAbility ? CrysGameplayAbility->GetAbilityName() : FText());
	SetIcon(CrysGameplayAbility ? CrysGameplayAbility->GetIcon() : nullptr);
}

void UAbilityViewModel::OnAbilitySystemComponentSet(UCrimAbilitySystemComponent* NewASC, UCrimAbilitySystemComponent* OldASC)
{
	if (OldASC)
	{
		OldASC->OnAbilityGivenDelegate.RemoveAll(this);
		OldASC->OnAbilityRemovedDelegate.RemoveAll(this);
		UnbindToAbilityCooldownTags(OldASC);
	}
	
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->OnAbilityGivenDelegate.AddUObject(this, &UAbilityViewModel::OnAbilityGiven);
		AbilitySystemComponent->OnAbilityRemovedDelegate.AddUObject(this, &UAbilityViewModel::OnAbilityRemoved);
	}
}

void UAbilityViewModel::OnAbilityGiven(const FGameplayAbilitySpec& Spec)
{
	if (AbilityClass == Spec.Ability->GetClass())
	{
		SetGameplayAbility(Spec, AbilitySystemComponent);
	}
}

void UAbilityViewModel::OnAbilityRemoved(const FGameplayAbilitySpec& Spec)
{
	if (AbilityClass == Spec.Ability->GetClass())
	{
		SetGameplayAbility(AbilityClass, AbilitySystemComponent);
	}
}

FGameplayTagContainer UAbilityViewModel::GenerateCooldownTags() const
{
	if (Ability)
	{
		return *Ability->GetCooldownTags();
	}
	return FGameplayTagContainer();
}

void UAbilityViewModel::TryBindToAbilityCooldownTags()
{
	if (AbilitySystemComponent)
	{
		UnbindToAbilityCooldownTags(AbilitySystemComponent);
		
		CooldownTags = GenerateCooldownTags();
		if (CooldownTags.Num() > 0)
		{
			for (const FGameplayTag& Tag : CooldownTags)
			{
				HandleCooldownTagCountChanged(Tag, AbilitySystemComponent->GetGameplayTagCount(Tag));
				BoundCooldownTagsASCHandles.Add(Tag, AbilitySystemComponent->RegisterGameplayTagEvent(Tag, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UAbilityViewModel::HandleCooldownTagCountChanged));
			}
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

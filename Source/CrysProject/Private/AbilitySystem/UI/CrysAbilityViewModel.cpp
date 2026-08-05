// Copyright Soccertitan 2026


#include "AbilitySystem/UI/CrysAbilityViewModel.h"

#include "CrimAbilitySystemComponent.h"
#include "AbilitySystem/Ability/CrysGameplayAbility.h"


float UCrysAbilityViewModel::GetCooldownTimeRemaining() const
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

void UCrysAbilityViewModel::SetGameplayAbility(const FGameplayAbilitySpec& Spec, UCrimAbilitySystemComponent* NewAbilitySystemComponent)
{
	SetAbilitySystemComponent(NewAbilitySystemComponent);
	UCrysGameplayAbility* NewAbility = Cast<UCrysGameplayAbility>(Spec.GetPrimaryInstance());
	if (!NewAbility)
	{
		NewAbility = Cast<UCrysGameplayAbility>(Spec.Ability);
	}
	SetIsAbilityGranted(NewAbility ? true : false);
	SetGameplayAbility(NewAbility);
}

void UCrysAbilityViewModel::SetGameplayAbility(TSubclassOf<UCrysGameplayAbility> InAbilityClass, UCrimAbilitySystemComponent* NewAbilitySystemComponent)
{
	SetAbilitySystemComponent(NewAbilitySystemComponent);
	
	UCrysGameplayAbility* NewAbility = nullptr;
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
		NewAbility = NewObject<UCrysGameplayAbility>(this, InAbilityClass);
	}
	SetIsAbilityGranted(false);
	SetGameplayAbility(NewAbility);
}

void UCrysAbilityViewModel::SetGameplayAbility(UCrysGameplayAbility* InAbility)
{
	if (Ability != InAbility)
	{
		UCrysGameplayAbility* OldAbility = Ability;
		Ability = InAbility;
		AbilityClass = Ability ? Ability->GetClass() : nullptr;
		TryBindToAbilityCooldownTags();
		OnGameplayAbilitySet(Ability, OldAbility);
	}
}

void UCrysAbilityViewModel::SetAbilitySystemComponent(UCrimAbilitySystemComponent* NewAbilitySystemComponent)
{
	if (AbilitySystemComponent != NewAbilitySystemComponent)
	{
		UCrimAbilitySystemComponent* OldAbilitySystemComponent = AbilitySystemComponent;
		AbilitySystemComponent = NewAbilitySystemComponent;
		OnAbilitySystemComponentSet(AbilitySystemComponent, OldAbilitySystemComponent);
	}
}

void UCrysAbilityViewModel::SetAbilityName(const FText& NewValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(AbilityName, NewValue);
}

void UCrysAbilityViewModel::SetIcon(const TSoftObjectPtr<UTexture2D>& NewValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(Icon, NewValue);
}

void UCrysAbilityViewModel::SetIsAbilityGranted(const bool NewValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(bAbilityGranted, NewValue);
}

void UCrysAbilityViewModel::SetIsAbilityOnCooldown(const bool NewValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(bAbilityOnCooldown, NewValue);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetCooldownTimeRemaining);
}

void UCrysAbilityViewModel::OnGameplayAbilitySet(UCrysGameplayAbility* NewAbility, UCrysGameplayAbility* OldAbility)
{
	if (NewAbility)
	{
		SetAbilityName(NewAbility->GetAbilityName());
		SetIcon(NewAbility->GetIcon());
	}
	else
	{
		SetAbilityName(FText());
		SetIcon(nullptr);
	}
}

void UCrysAbilityViewModel::OnAbilitySystemComponentSet(UCrimAbilitySystemComponent* NewASC, UCrimAbilitySystemComponent* OldASC)
{
	if (OldASC)
	{
		OldASC->OnAbilityGivenDelegate.RemoveAll(this);
		OldASC->OnAbilityRemovedDelegate.RemoveAll(this);
		UnbindToAbilityCooldownTags(OldASC);
	}
	
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->OnAbilityGivenDelegate.AddUObject(this, &UCrysAbilityViewModel::OnAbilityGiven);
		AbilitySystemComponent->OnAbilityRemovedDelegate.AddUObject(this, &UCrysAbilityViewModel::OnAbilityRemoved);
	}
}

void UCrysAbilityViewModel::OnAbilityGiven(const FGameplayAbilitySpec& Spec)
{
	if (AbilityClass == Spec.Ability->GetClass())
	{
		SetGameplayAbility(Spec, AbilitySystemComponent);
	}
}

void UCrysAbilityViewModel::OnAbilityRemoved(const FGameplayAbilitySpec& Spec)
{
	if (AbilityClass == Spec.Ability->GetClass())
	{
		SetGameplayAbility(AbilityClass, AbilitySystemComponent);
	}
}

FGameplayTagContainer UCrysAbilityViewModel::GenerateCooldownTags() const
{
	if (Ability)
	{
		return *Ability->GetCooldownTags();
	}
	return FGameplayTagContainer();
}

void UCrysAbilityViewModel::TryBindToAbilityCooldownTags()
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
				BoundCooldownTagsASCHandles.Add(Tag, AbilitySystemComponent->RegisterGameplayTagEvent(Tag, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UCrysAbilityViewModel::HandleCooldownTagCountChanged));
			}
		}
	}
}

void UCrysAbilityViewModel::UnbindToAbilityCooldownTags(UCrimAbilitySystemComponent* ASC)
{
	for (auto& Elem : BoundCooldownTagsASCHandles)
	{
		ASC->RegisterGameplayTagEvent(Elem.Key, EGameplayTagEventType::NewOrRemoved).Remove(Elem.Value);
	}
}

void UCrysAbilityViewModel::HandleCooldownTagCountChanged(const FGameplayTag GameplayTag, int32 Count)
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

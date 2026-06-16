// Copyright Soccertitan 2026


#include "AbilitySystem/UI/AbilityViewModel.h"

#include "AbilitySystem/GameplayAbilityData.h"

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
		UpdateViewModelData(GameplayAbilityData);
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

void UAbilityViewModel::UpdateViewModelData(UGameplayAbilityData* AbilityData)
{
	SetAbilityName(AbilityData->AbilityName);
	SetIcon(AbilityData->Icon);
}

// Copyright Soccertitan 2026


#include "AbilitySystem/UI/AbilityViewModel.h"

void UAbilityViewModel::SetAbilityName(const FText& NewValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(AbilityName, NewValue);
}

void UAbilityViewModel::SetIcon(const TSoftObjectPtr<UTexture2D>& NewValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(Icon, NewValue);
}

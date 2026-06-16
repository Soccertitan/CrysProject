// Copyright Soccertitan 2026


#include "AbilitySystem/GameplayAbilityData.h"

#include "AbilitySystem/UI/AbilityViewModel.h"

UGameplayAbilityData::UGameplayAbilityData()
{
	AbilityViewModel = UAbilityViewModel::StaticClass();
}

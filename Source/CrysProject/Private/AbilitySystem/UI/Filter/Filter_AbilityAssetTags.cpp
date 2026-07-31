// Copyright Soccertitan 2026


#include "AbilitySystem/UI/Filter/Filter_AbilityAssetTags.h"

#include "Abilities/GameplayAbility.h"
#include "AbilitySystem/UI/AbilityViewModel.h"

bool UFilter_AbilityAssetTags::ShouldFilterObject_Implementation(const UObject* Object, const UObject* Context) const
{
	const UAbilityViewModel* ViewModel = Cast<UAbilityViewModel>(Object);
	
	if (!ViewModel)
	{
		return true;
	}
	
	const UGameplayAbility* Ability = ViewModel->GetGameplayAbility();
	if (!Ability)
	{
		return true;
	}
	
	return !Ability->GetAssetTags().HasAny(AbilityAssetTags);
}

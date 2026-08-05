// Copyright Soccertitan 2026


#include "AbilitySystem/UI/Filter/Filter_AbilityAssetTags.h"

#include "AbilitySystem/Ability/CrysGameplayAbility.h"
#include "AbilitySystem/UI/CrysAbilityViewModel.h"

bool UFilter_AbilityAssetTags::ShouldFilterObject_Implementation(const UObject* Object, const UObject* Context) const
{
	const UCrysAbilityViewModel* ViewModel = Cast<UCrysAbilityViewModel>(Object);
	
	if (!ViewModel)
	{
		return true;
	}
	
	const UCrysGameplayAbility* Ability = ViewModel->GetGameplayAbility();
	if (!Ability)
	{
		return true;
	}
	
	return !Ability->GetAssetTags().HasAny(AbilityAssetTags);
}

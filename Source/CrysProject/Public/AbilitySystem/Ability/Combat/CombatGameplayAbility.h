// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/CrysGameplayAbility.h"
#include "CombatGameplayAbility.generated.h"

/**
 * Contains utility functions for responding to combat related queries.
 */
UCLASS(Abstract)
class CRYSPROJECT_API UCombatGameplayAbility : public UCrysGameplayAbility
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, Category = "Ability|Combat")
	bool IsDualWielding() const { return bDualWielding;}
	
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	
private:
	bool bDualWielding;
	
	
};

// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

namespace Crys::NativeGameplayTag
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_State_MovementRooted);
	
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_GameplayEvent_Damage_ManaPoints);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_GameplayEvent_Healing_ManaPoints);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_GameplayEvent_Damage_TacticalPoints);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_GameplayEvent_Healing_TacticalPoints);
	
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_State_Block);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_State_CombatStance);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_State_Guard);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_State_Ignore_Evasion);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_State_Parry);
	
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(EnhancedInput_UI);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(EnhancedInput_AbilityTargeting);
	
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Job);
}

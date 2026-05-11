// Copyright Soccertitan 2026


#include "CrysNativeGameplayTags.h"
#include "NativeGameplayTags.h"

namespace Crys::NativeGameplayTag
{
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_State_MovementRooted, "Ability.State.MovementRooted", "The character cannot move or rotate.");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_GameplayEvent_Damage_ManaPoints, "Ability.GameplayEvent.Damage.ManaPoints", "Called when the ManaPoints attribute set has damage set.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_GameplayEvent_Healing_ManaPoints, "Ability.GameplayEvent.Healing.ManaPoints", "Called when the ManaPoints attribute set has healing set.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_GameplayEvent_Damage_TacticalPoints, "Ability.GameplayEvent.Damage.TacticalPoints", "Called when the TacticalPoints attribute set has damage set.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_GameplayEvent_Healing_TacticalPoints, "Ability.GameplayEvent.Healing.TacticalPoints", "Called when the TacticalPoints attribute set has healing set.");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_State_Block, "Ability.State.Block", "A character can block attacks with this tag.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_State_CombatStance, "Ability.State.CombatStance", "A character with this tag have their weapons drawn.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_State_Guard, "Ability.State.Guard", "A character can guard from attacks with this tag.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_State_Ignore_Evasion, "Ability.State.Ignore.Evasion", "Effects with tag ignores the Target's evasion attribute.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_State_Parry, "Ability.State.PauseAutoAttack", "A character with this tag will have the auto attack timer paused. And restarted when removed.");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(EnhancedInput_UI, "EnhancedInput.UI", "When a UINav widget is added to the screen. This input mode is added.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(EnhancedInput_AbilityTargeting, "EnhancedInput.AbilityTargeting", "When an ability starts targeting.");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Job, "Job", "Jobs and races characters can be.");
}

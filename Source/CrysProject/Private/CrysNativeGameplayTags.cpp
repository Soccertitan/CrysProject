// Copyright Soccertitan 2026


#include "CrysNativeGameplayTags.h"
#include "NativeGameplayTags.h"

namespace Crys::NativeGameplayTag
{
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Combat_CombatStance, "Ability.Combat.CombatStance", "The ability to switch combat stances.");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_State_MovementRooted, "Ability.State.MovementRooted", "The character cannot move or rotate.");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_GameplayEvent_AutoAttack, "Ability.GameplayEvent.AutoAttack", "Triggers an auto attack gameplay ability.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_GameplayEvent_Damage_ManaPoints, "Ability.GameplayEvent.Damage.ManaPoints", "Called when the ManaPoints attribute set has damage set.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_GameplayEvent_Healing_ManaPoints, "Ability.GameplayEvent.Healing.ManaPoints", "Called when the ManaPoints attribute set has healing set.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_GameplayEvent_Damage_TacticalPoints, "Ability.GameplayEvent.Damage.TacticalPoints", "Called when the TacticalPoints attribute set has damage set.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_GameplayEvent_Healing_TacticalPoints, "Ability.GameplayEvent.Healing.TacticalPoints", "Called when the TacticalPoints attribute set has healing set.");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_State_AutoAttackPaused, "Ability.State.AutoAttackPaused", "The AutoAttack timer is paused while this tag is added to an ASC.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_State_Block, "Ability.State.Block", "A character can block attacks with this tag.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_State_CombatStance, "Ability.State.CombatStance", "A character with this tag have their weapons drawn.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_State_DualWield, "Ability.State.DualWield", "The character may equip two one-handed weapons.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_State_DualWielding, "Ability.State.DualWielding", "The character has two one-handed weapons equipped.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_State_Guard, "Ability.State.Guard", "A character can guard from attacks with this tag.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_State_Ignore_Evasion, "Ability.State.Ignore.Evasion", "Effects with tag ignores the Target's evasion attribute.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_State_Parry, "Ability.State.PauseAutoAttack", "A character with this tag will have the auto attack timer paused. And restarted when removed.");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute, "Attribute", "Root tag for GAS attributes.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Level, "Attribute.Level", "The level of the character.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Strength, "Attribute.Strength", "Boosts physical damage.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Vitality, "Attribute.Vitality", "Increases maximum HP and reduces physical damage.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Dexterity, "Attribute.Dexterity", "Increases the hit and critical hit rate.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Agility, "Attribute.Agility", "Increases evasion and reduces the chance to be hit by a critical strike.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Intelligence, "Attribute.Intelligence", "Increases offensive magic damage.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Mind, "Attribute.Mind", "Increases MP, reduces offensive magic damage, and increases healing effectiveness.");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_MultiAttackChance, "Attribute.MultiAttackChance", "Root tag for Multi Attack Chances.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_MultiAttackChance_Double, "Attribute.MultiAttackChance.Double", "Two attacks for auto attacks");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_MultiAttackChance_Triple, "Attribute.MultiAttackChance.Triple", "Three attacks for auto attacks.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_MultiAttackChance_Quadruple, "Attribute.MultiAttackChance.Quadruple", "Four attacks for auto attacks.");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(EnhancedInput_Modes_AbilityTargeting, "EnhancedInput.Modes.AbilityTargeting", "When an ability starts targeting.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(EnhancedInput_Modes_UI, "EnhancedInput.Modes.UI", "When a UINav widget is added to the screen. This input mode is added.");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(EquipSlot, "EquipSlot", "Root GameplayTag for equipment slots.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(EquipSlot_Hand, "EquipSlot.Hand", "The equipment can be wielded in both the MainHand and SubHand.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(EquipSlot_Hand_Main, "EquipSlot.Hand.Main", "Main Hand slot that can only hold a weapon.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(EquipSlot_Hand_Sub, "EquipSlot.Hand.Sub", "Sub Hand slot for dual wielding, shield, ammo, or some other sub hand equipment");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Job, "Job", "Jobs and races characters can be.");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(SetByCaller_BasePotency, "SetByCaller.BasePotency", "Abilities will use this tag to pass the base potency to the gameplay effect spec.");
}

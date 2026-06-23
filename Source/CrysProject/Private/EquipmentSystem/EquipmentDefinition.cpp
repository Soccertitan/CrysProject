// Copyright Soccertitan 2026


#include "EquipmentSystem/EquipmentDefinition.h"

#include "CrysNativeGameplayTags.h"


#if WITH_EDITOR
void UEquipmentDefinition::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	
	// Ensure that the EquipSlot tags are not also in the BlockedEquipSlots.
	BlockEquipSlots.RemoveTag(EquipSlot);

	if (bWeapon)
	{
		FGameplayTagContainer AllowedWeaponSlots;
		AllowedWeaponSlots.AddTagFast(Crys::NativeGameplayTag::EquipSlot_Hand_Main);
		// Ensure weapons can only have the Hand.Main or Hand tag.
		if (!AllowedWeaponSlots.HasTag(EquipSlot))
		{
			EquipSlot = Crys::NativeGameplayTag::EquipSlot_Hand;
		}
	}
	else
	{
		// We're not a weapon. So prevent this equipment from being allowed to be held in the main hand.
		if (EquipSlot == Crys::NativeGameplayTag::EquipSlot_Hand_Main ||
			EquipSlot == Crys::NativeGameplayTag::EquipSlot_Hand)
		{
			EquipSlot = FGameplayTag();
		}
	}
}
#endif


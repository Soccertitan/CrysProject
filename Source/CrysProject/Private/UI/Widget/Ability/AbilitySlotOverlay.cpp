// Copyright Soccertitan 2026


#include "UI/Widget/Ability/AbilitySlotOverlay.h"

#include "UI/Widget/Ability/AbilitySlotWidget.h"

void UAbilitySlotOverlay::SetAbilitySlotWidget(UAbilitySlotWidget* AbilitySlotWidget)
{
	ClearChildren();
	AbilitySlotWidget->InputSlot = InputSlot;
	AddChildToOverlay(AbilitySlotWidget);
}

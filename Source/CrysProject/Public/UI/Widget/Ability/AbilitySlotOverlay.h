// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "Components/Overlay.h"
#include "Input/AbilityInputSlot.h"
#include "AbilitySlotOverlay.generated.h"

class UAbilitySlotWidget;
/**
 * Used by an AbilityContainerWidget to create an AbilitySlotWidget in this overlay slot matching the InputSlot.
 */
UCLASS()
class CRYSPROJECT_API UAbilitySlotOverlay : public UOverlay
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn))
	FAbilityInputSlot InputSlot;
	
	void SetAbilitySlotWidget(UAbilitySlotWidget* AbilitySlotWidget);
};

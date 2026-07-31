// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "UINavComponent.h"
#include "Input/AbilityInputSlot.h"
#include "AbilitySlotWidget.generated.h"


class UAbilityInputSlotViewModel;

/**
 * 
 */
UCLASS(Abstract)
class CRYSPROJECT_API UAbilitySlotWidget : public UUINavComponent
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn))
	FAbilityInputSlot InputSlot;
	
	UFUNCTION(BlueprintImplementableEvent)
	void K2_SetAbilityInputSlotViewModel(UAbilityInputSlotViewModel* ViewModel);
};

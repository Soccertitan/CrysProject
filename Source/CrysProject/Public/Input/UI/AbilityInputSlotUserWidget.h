// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/CrysUserWidget.h"
#include "AbilityInputSlotUserWidget.generated.h"

class UCrysAbilityViewModel;
/**
 * A specialized user widget to display information about an ability in a hotbar slot.
 */
UCLASS()
class CRYSPROJECT_API UAbilityInputSlotUserWidget : public UCrysUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "SetAbilityViewModel")
	void K2_SetAbilityViewModel(UCrysAbilityViewModel* NewViewModel);
};

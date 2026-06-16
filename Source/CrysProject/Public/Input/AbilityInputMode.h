// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "AbilityInputMode.generated.h"


UENUM(BlueprintType)
enum class EAbilityInputMode : uint8
{
	Gamepad UMETA(DisplayName = "Gamepad"),
	Keyboard UMETA(DisplayName = "Keyboard")
};

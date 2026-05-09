// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "AbilityTargetType.generated.h"

/** Defines who the ability can target. */
UENUM(BlueprintType)
enum class EAbilityTargetType : uint8
{
	Any,
	SelfOnly,
	FriendlyExcludeSelf,
	Friendly,
	Hostile
};

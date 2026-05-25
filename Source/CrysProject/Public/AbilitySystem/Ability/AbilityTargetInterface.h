// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AbilityTargetInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UAbilityTargetInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Returns an actor to target with an ability.
 */
class CRYSPROJECT_API IAbilityTargetInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent)
	AActor* GetAbilityTarget() const;
};

// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatSystemInterface.generated.h"

class UCombatSystemComponent;
// This class does not need to be modified.
UINTERFACE()
class UCombatSystemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CRYSPROJECT_API ICombatSystemInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent)
	UCombatSystemComponent* GetCombatSystemComponent() const;
};

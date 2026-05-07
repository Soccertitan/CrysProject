// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "JobSystemInterface.generated.h"

class UJobSystemComponent;
// This class does not need to be modified.
UINTERFACE()
class UJobSystemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CRYSPROJECT_API IJobSystemInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent)
	UJobSystemComponent* GetJobSystemComponent() const;
};

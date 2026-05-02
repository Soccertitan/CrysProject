// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InputActionListenerMap.generated.h"

class UInputActionListener;
class UInputAction;

/** Maps an InputAction to an InputActionListener. */
USTRUCT(BlueprintType)
struct FInputActionListenerData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UInputAction> InputAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UInputActionListener> InputActionListener;
	
	friend bool operator==(const FInputActionListenerData& X, const FInputActionListenerData& Y)
	{
		return X.InputAction == Y.InputAction;
	}
	
	friend bool operator!=(const FInputActionListenerData& X, const FInputActionListenerData& Y)
	{
		return X.InputAction != Y.InputAction;
	}
};

/**
 * Maps an InputAction to an InputActionListener.
 */
UCLASS(Const)
class CRYSPROJECT_API UInputActionListenerMap : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (NoResetToDefault, NoElementDuplicate))
	TArray<FInputActionListenerData> InputActionListenerMap;
};

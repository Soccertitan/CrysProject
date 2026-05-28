// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "CrysInputActionListener.h"
#include "AbilitySystem/AbilityTargetType.h"
#include "InputActionListener_CycleTarget.generated.h"

struct FTargetingRequestHandle;
class UCrimTargetingSystemComponent;
class UTargetingPreset;
class UCrysTargetingContext;
/**
 * Targets an actor using the TargetingSubsystem.
 */
UCLASS()
class CRYSPROJECT_API UInputActionListener_CycleTarget : public UCrysInputActionListener
{
	GENERATED_BODY()
	
public:
	UInputActionListener_CycleTarget();
	
protected:
	UPROPERTY(EditAnywhere)
	EAbilityTargetType AbilityTargetType = EAbilityTargetType::Hostile;
	
	UFUNCTION(BlueprintNativeEvent)
	EAbilityTargetType GetAbilityTargetType() const;
	virtual EAbilityTargetType GetAbilityTargetType_Implementation() const;
	
	virtual void OnInitializeListener() override;
	virtual void OnInputActionTriggered(const FInputActionValue& Value) override;
	
	UFUNCTION()
	void OnTargetingRequestCompleted(FTargetingRequestHandle Handle);
	
private:
	UPROPERTY(EditAnywhere, NoClear)
	TObjectPtr<UTargetingPreset> TargetingPreset;
	UPROPERTY()
	TObjectPtr<UCrysTargetingContext> TargetingContext;
	
	UPROPERTY()
	TObjectPtr<UCrimTargetingSystemComponent> TargetingSystemComponent;
};

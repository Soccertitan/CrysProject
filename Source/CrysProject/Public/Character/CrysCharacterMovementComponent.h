// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "CrimAbilitySystemInterface.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CrysCharacterMovementComponent.generated.h"


struct FGameplayTag;
struct FOnAttributeChangeData;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CRYSPROJECT_API UCrysCharacterMovementComponent : public UCharacterMovementComponent, public ICrimAbilitySystemInterface
{
	GENERATED_BODY()

public:
	UCrysCharacterMovementComponent();

	virtual float GetMaxSpeed() const override;
	virtual FRotator GetDeltaRotation(float DeltaTime) const override;

	// ICrimAbilitySystemInterface
	virtual void InitializeWithAbilitySystem_Implementation(UCrimAbilitySystemComponent* NewAbilitySystemComponent) override;
	
private:
	// Cached ASC from the owner.
	UPROPERTY()
	TObjectPtr<UCrimAbilitySystemComponent> AbilitySystemComponent;
	
	// Cached value from the ASC.
	UPROPERTY()
	float MovementSpeedMultiplier = 1.f;
	
	UPROPERTY()
	bool bMovementRooted = false;
	
	void OnGameplayTagMovementRootedUpdated(const FGameplayTag Tag, int32 NewCount);
	void OnMovementSpeedMultiplierUpdated(const FOnAttributeChangeData& Data);
};

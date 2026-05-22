// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "CrimAbilitySystemInterface.h"
#include "LockOnPawnInterface.h"
#include "LockOnTypes.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CrysCharacterMovementComponent.generated.h"


struct FGameplayTag;
struct FOnAttributeChangeData;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CRYSPROJECT_API UCrysCharacterMovementComponent : public UCharacterMovementComponent, public ICrimAbilitySystemInterface,
	public ILockOnPawnInterface
{
	GENERATED_BODY()

public:
	UCrysCharacterMovementComponent();

	virtual float GetMaxSpeed() const override;
	virtual FRotator GetDeltaRotation(float DeltaTime) const override;
	virtual FRotator ComputeOrientToMovementRotation(const FRotator& CurrentRotation, float DeltaTime, FRotator& DeltaRotation) const override;

	// ICrimAbilitySystemInterface
	virtual void SetCrimAbilitySystem_Implementation(UCrimAbilitySystemComponent* AbilitySystemComponent) override;
	// ILockOnPawnInterface
	virtual void SetLockOnPoint_Implementation(const FCrimTargetPoint& NewLockOnPoint) override;
	
private:
	// Cached ASC from the owner.
	UPROPERTY()
	TObjectPtr<UCrimAbilitySystemComponent> AbilitySystemComponent;
	
	// Cached value from the ASC.
	UPROPERTY()
	float MovementSpeedMultiplier = 1.f;
	
	UPROPERTY()
	bool bMovementRooted = false;
	
	// The point to rotate towards whe moving.
	UPROPERTY()
	FCrimTargetPoint LockOnPoint;
	
	void OnGameplayTagMovementRootedUpdated(const FGameplayTag Tag, int32 NewCount);
	void OnMovementSpeedMultiplierUpdated(const FOnAttributeChangeData& Data);
};

// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "CrimAbilitySystemInterface.h"
#include "Components/SkeletalMeshComponent.h"
#include "CrysSkeletalMeshComponent.generated.h"

/**
 * 
 */
UCLASS()
class CRYSPROJECT_API UCrysSkeletalMeshComponent : public USkeletalMeshComponent, public ICrimAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	// ICrimAbilitySystemInterface
	virtual void SetCrimAbilitySystem_Implementation(UCrimAbilitySystemComponent* NewAbilitySystemComponent) override;
};

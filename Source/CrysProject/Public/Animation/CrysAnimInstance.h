// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CrimAbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "Animation/AnimInstance.h"
#include "CrysAnimInstance.generated.h"


/**
 * The base animation instance class for this project. Interfaces with the Gameplay Ability System
 */
UCLASS()
class CRYSPROJECT_API UCrysAnimInstance : public UAnimInstance, public ICrimAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	UCrysAnimInstance(const FObjectInitializer& ObjectInitializer);

	//ICrimAbilitySystemInterface
	virtual void InitializeWithAbilitySystem_Implementation(UCrimAbilitySystemComponent* AbilitySystemComponent) override;

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif
	
	virtual void NativeInitializeAnimation() override;
	
protected:
	/**
	 * Gameplay tags that can be mapped to blueprint variables. The variables will automatically update as the tags
	 * are added or removed. These should be used instead of manually querying for the gameplay tags.
	 */
	UPROPERTY(EditDefaultsOnly, Category = "GameplayTags")
	FGameplayTagBlueprintPropertyMap GameplayTagPropertyMap;
};

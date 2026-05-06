// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/CrysAnimInstance.h"

#include "CrimAbilitySystemBlueprintFunctionLibrary.h"
#include "CrimAbilitySystemComponent.h"
#include "CrimAbilitySystemInterface.h"
#include "Misc/DataValidation.h"

#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

UCrysAnimInstance::UCrysAnimInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UCrysAnimInstance::SetCrimAbilitySystem_Implementation(UCrimAbilitySystemComponent* AbilitySystemComponent)
{
	check(AbilitySystemComponent);
	GameplayTagPropertyMap.Initialize(this, AbilitySystemComponent);
	GameplayTagPropertyMap.ApplyCurrentTags();
}

#if WITH_EDITOR
EDataValidationResult UCrysAnimInstance::IsDataValid(class FDataValidationContext& Context) const
{
	Super::IsDataValid(Context);
	
	GameplayTagPropertyMap.IsDataValid(this, Context);

	return Context.GetNumErrors() > 0 ? EDataValidationResult::Invalid : EDataValidationResult::Valid;
}
#endif

void UCrysAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	if (AActor* OwningActor = GetOwningActor())
	{
		if (UCrimAbilitySystemComponent* AbilitySystemComponent = UCrimAbilitySystemBlueprintFunctionLibrary::GetAbilitySystemComponent(OwningActor))
		{
			ICrimAbilitySystemInterface::Execute_SetCrimAbilitySystem(this, AbilitySystemComponent);
		}
	}
}

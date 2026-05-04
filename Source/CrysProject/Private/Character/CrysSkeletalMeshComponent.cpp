// Copyright Soccertitan 2026


#include "Character/CrysSkeletalMeshComponent.h"

void UCrysSkeletalMeshComponent::InitializeWithAbilitySystem_Implementation(UCrimAbilitySystemComponent* NewAbilitySystemComponent)
{
	if (UAnimInstance* AnimInstance = GetAnimInstance())
	{
		if (AnimInstance->Implements<UCrimAbilitySystemInterface>())
		{
			ICrimAbilitySystemInterface::Execute_InitializeWithAbilitySystem(AnimInstance, NewAbilitySystemComponent);
		}
	}
}
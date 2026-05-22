// Copyright Soccertitan 2026


#include "AbilitySystem/CrysAbilityBlueprintFunctionLibrary.h"

#include "CrysBlueprintFunctionLibrary.h"
#include "AbilitySystem/AbilityTargetType.h"

bool UCrysAbilityBlueprintFunctionLibrary::IsAbilityTargetType(EAbilityTargetType AbilityTargetType, AActor* SourceActor, AActor* TargetActor)
{
	if (AbilityTargetType == EAbilityTargetType::SelfOnly)
	{
		return SourceActor == TargetActor;
	}
		
	if (AbilityTargetType == EAbilityTargetType::FriendlyExcludeSelf)
	{
		if (SourceActor == TargetActor)
		{
			return false;
		}
			
		return ETeamAttitude::Friendly == UCrysBlueprintFunctionLibrary::GetAttitudeTowardsActor(SourceActor, TargetActor);
	}
		
	if (AbilityTargetType == EAbilityTargetType::Friendly)
	{
		return ETeamAttitude::Friendly == UCrysBlueprintFunctionLibrary::GetAttitudeTowardsActor(SourceActor, TargetActor);
	}
		
	if (AbilityTargetType == EAbilityTargetType::Hostile)
	{
		return ETeamAttitude::Hostile == UCrysBlueprintFunctionLibrary::GetAttitudeTowardsActor(SourceActor, TargetActor);
	}
	
	return true;
}

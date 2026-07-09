// Copyright Soccertitan 2026


#include "AbilitySystem/CrysAbilityBlueprintFunctionLibrary.h"

#include "CrysBlueprintFunctionLibrary.h"
#include "CrysLogChannels.h"
#include "AbilitySystem/AbilityTargetType.h"
#include "AbilitySystem/GameplayTagInfoFragment_Attribute.h"
#include "AbilitySystem/Ability/AbilityTargetInterface.h"
#include "System/CrysGameplayTagRelationship.h"


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

AActor* UCrysAbilityBlueprintFunctionLibrary::GetAbilityTarget(AActor* SourceActor)
{
	if (SourceActor && SourceActor->Implements<UAbilityTargetInterface>())
	{
		return IAbilityTargetInterface::Execute_GetAbilityTarget(SourceActor);
	}
	return nullptr;
}

const FCrysGameplayTagInfo* UCrysAbilityBlueprintFunctionLibrary::FindAttributeGameplayTagInfo(
	const UCrysGameplayTagRelationship* GameplayTagRelationship, const FGameplayAttribute& GameplayAttribute,
	const FGameplayTagContainer& SourceTags, bool bLogNotFound)
{
	if (GameplayTagRelationship)
	{
		for (const FCrysGameplayTagInfo& GameplayTagInfo : GameplayTagRelationship->GameplayTagInfos)
		{
			if (const FGameplayTagInfoFragment_Attribute* Fragment = GameplayTagInfo.FindFragmentByType<FGameplayTagInfoFragment_Attribute>())
			{
				if (Fragment->GameplayAttribute == GameplayAttribute && Fragment->SourceTags == SourceTags)
				{
					return &GameplayTagInfo;
				}
			}
		}
		if (bLogNotFound)
		{
			UE_LOG(LogCrys, Error, TEXT("Cannot find info for GameplayAttribute [%s] and SourceTags [%s] in [%s]"), *GameplayAttribute.AttributeName, *SourceTags.ToString(), *GetNameSafe(GameplayTagRelationship));
		}
	}
	return nullptr;
}

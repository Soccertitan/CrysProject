// Copyright Soccertitan 2026


#include "TargetingSystem/Task/TargetingFilterTask_AbilityTargetType.h"

#include "AbilitySystem/CrysAbilityBlueprintFunctionLibrary.h"


bool UTargetingFilterTask_AbilityTargetType::ShouldFilterTarget(const FTargetingRequestHandle& TargetingHandle,const FTargetingDefaultResultData& TargetData) const
{
	if (FTargetingSourceContext* Context = FTargetingSourceContext::Find(TargetingHandle))
	{
		return !UCrysAbilityBlueprintFunctionLibrary::IsAbilityTargetType(AbilityTargetType, Context->SourceActor, TargetData.HitResult.GetActor());
	}
	return false;
}

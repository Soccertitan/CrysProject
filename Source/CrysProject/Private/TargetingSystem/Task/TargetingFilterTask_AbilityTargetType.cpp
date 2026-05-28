// Copyright Soccertitan 2026


#include "TargetingSystem/Task/TargetingFilterTask_AbilityTargetType.h"

#include "AbilitySystem/CrysAbilityBlueprintFunctionLibrary.h"
#include "TargetingSystem/CrysTargetingContext.h"


bool UTargetingFilterTask_AbilityTargetType::ShouldFilterTarget(const FTargetingRequestHandle& TargetingHandle,const FTargetingDefaultResultData& TargetData) const
{
	if (FTargetingSourceContext* Context = FTargetingSourceContext::Find(TargetingHandle))
	{
		return !UCrysAbilityBlueprintFunctionLibrary::IsAbilityTargetType(GetAbilityTargetType(TargetingHandle), Context->SourceActor, TargetData.HitResult.GetActor());
	}
	return false;
}

EAbilityTargetType UTargetingFilterTask_AbilityTargetType::GetAbilityTargetType(const FTargetingRequestHandle& TargetingHandle) const
{
	if (FTargetingSourceContext* Context = FTargetingSourceContext::Find(TargetingHandle))
	{
		if (UCrysTargetingContext* CrysTargetingContext = Cast<UCrysTargetingContext>(Context->SourceObject))
		{
			return CrysTargetingContext->AbilityTargetType;
		}
	}
	return AbilityTargetType;
}

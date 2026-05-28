// Copyright Soccertitan 2026


#include "TargetingSystem/Task/CrysTargetingSelectionTask_FindTarget.h"

#include "TargetingSystem/CrysTargetingContext.h"

ETargetingSearchDirection UCrysTargetingSelectionTask_FindTarget::GetSearchDirection(const FTargetingRequestHandle& TargetingHandle) const
{
	if (FTargetingSourceContext* Context = FTargetingSourceContext::Find(TargetingHandle))
	{
		if (UCrysTargetingContext* CrysTargetingContext = Cast<UCrysTargetingContext>(Context->SourceObject))
		{
			return CrysTargetingContext->SearchDirection;
		}
	}
	return Super::GetSearchDirection(TargetingHandle);
}

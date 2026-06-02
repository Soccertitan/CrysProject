// Copyright Soccertitan 2026


#include "TargetingSystem/Task/CrysTargetingSelectionTask_FindTarget.h"

#include "TargetingSystem/CrysTargetingContext.h"


FVector2D UCrysTargetingSelectionTask_FindTarget::GetSearchDirection(const FTargetingRequestHandle& TargetingHandle) const
{
	if (FTargetingSourceContext* Context = FTargetingSourceContext::Find(TargetingHandle))
	{
		if (UCrysTargetingContext* TargetingContext = Cast<UCrysTargetingContext>(Context->SourceObject))
		{
			return TargetingContext->SearchDirection;
		}
	}
	return Super::GetSearchDirection(TargetingHandle);
}

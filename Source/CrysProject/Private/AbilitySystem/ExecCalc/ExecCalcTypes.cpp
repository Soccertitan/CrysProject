// Copyright Soccertitan 2026


#include "AbilitySystem/ExecCalc/ExecCalcTypes.h"


bool FExecCalcEffectChance::CalculateIsSuccess(float Chance, const float Level, const FGameplayTagContainer& SourceTags,
	const FGameplayTagContainer& TargetTags) const
{
	const bool bTagRequirementsMet = SourceTagRequirements.RequirementsMet(SourceTags) && TargetTagRequirements.RequirementsMet(TargetTags);
	if (bTagRequirementsMet == false)
	{
		return false;
	}
	
	const bool bAutoSuccess = bUseSourceTagsForAutoSuccess ? AutoSuccessTagRequirements.RequirementsMet(SourceTags) : AutoSuccessTagRequirements.RequirementsMet(TargetTags);
	const bool bAutoFail = bUseSourceTagsForAutoSuccess ? AutoFailTagRequirements.RequirementsMet(TargetTags) : AutoFailTagRequirements.RequirementsMet(SourceTags);

	if (bAutoSuccessTagsHasPriority)
	{
		if (bAutoSuccess)
		{
			return true;
		}
		if (bAutoFail)
		{
			return false;
		}
	}
	else
	{
		if (bAutoFail)
		{
			return false;
		}
		if (bAutoSuccess)
		{
			return true;
		}
	}
	
	Chance = FMath::Clamp(Chance, MinChance.GetValueAtLevel(Level), MaxChance.GetValueAtLevel(Level));
	
	return Chance >= FMath::RandRange(0.f, 1.f);
}

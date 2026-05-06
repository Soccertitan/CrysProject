// Copyright Soccertitan 2026


#include "JobSystem/JobTypes.h"

#include "JobSystem/JobDefinition.h"


FPrimaryAttributesCalc::FPrimaryAttributesCalc(const UJobDefinition* Race, const int32 Level,
                                               const UJobDefinition* MainJob, const int32 MainJobLevel, const UJobDefinition* SubJob, const int32 SubJobLevel,
                                               const float SubJobEfficiency)
{
	const bool bRaceValid = Race ? true : false;
	const bool bMainJobValid = MainJob ? true : false;
	const bool bSubJobValid = SubJob ? true : false;
	
	MaxHitPoints = CalculateValue(
		bRaceValid ? Race->BaseAttributes.MaxHitPoints.GetValueAtLevel(Level) : 1,
		bMainJobValid ? MainJob->BaseAttributes.MaxHitPoints.GetValueAtLevel(MainJobLevel) : 1,
		bSubJobValid ? SubJob->BaseAttributes.MaxHitPoints.GetValueAtLevel(SubJobLevel) : 0,
		SubJobEfficiency);
	
	MaxMagicPoints = CalculateValue(bRaceValid ? Race->BaseAttributes.MaxMagicPoints.GetValueAtLevel(Level) : 1,
		bMainJobValid ? MainJob->BaseAttributes.MaxMagicPoints.GetValueAtLevel(MainJobLevel) : 1,
		bSubJobValid ? SubJob->BaseAttributes.MaxMagicPoints.GetValueAtLevel(SubJobLevel) : 0,
		SubJobEfficiency);
	
	Strength = CalculateValue(bRaceValid ? Race->BaseAttributes.Strength.GetValueAtLevel(Level) : 1,
		bMainJobValid ? MainJob->BaseAttributes.Strength.GetValueAtLevel(MainJobLevel) : 1,
		bSubJobValid ? SubJob->BaseAttributes.Strength.GetValueAtLevel(SubJobLevel) : 0,
		SubJobEfficiency);
	
	Vitality = CalculateValue(bRaceValid ? Race->BaseAttributes.Vitality.GetValueAtLevel(Level) : 1,
		bMainJobValid ? MainJob->BaseAttributes.Vitality.GetValueAtLevel(MainJobLevel) : 1,
		bSubJobValid ? SubJob->BaseAttributes.Vitality.GetValueAtLevel(SubJobLevel) : 0,
		SubJobEfficiency);
	
	Dexterity = CalculateValue(bRaceValid ? Race->BaseAttributes.Dexterity.GetValueAtLevel(Level) : 1,
		bMainJobValid ? MainJob->BaseAttributes.Dexterity.GetValueAtLevel(MainJobLevel) : 1,
		bSubJobValid ? SubJob->BaseAttributes.Dexterity.GetValueAtLevel(SubJobLevel) : 0,
		SubJobEfficiency);
	
	Agility = CalculateValue(bRaceValid ? Race->BaseAttributes.Agility.GetValueAtLevel(Level) : 1,
		bMainJobValid ? MainJob->BaseAttributes.Agility.GetValueAtLevel(MainJobLevel) : 1,
		bSubJobValid ? SubJob->BaseAttributes.Agility.GetValueAtLevel(SubJobLevel) : 0,
		SubJobEfficiency);
	
	Intelligence = CalculateValue(bRaceValid ? Race->BaseAttributes.Intelligence.GetValueAtLevel(Level) : 1,
		bMainJobValid ? MainJob->BaseAttributes.Intelligence.GetValueAtLevel(MainJobLevel) : 1,
		bSubJobValid ? SubJob->BaseAttributes.Intelligence.GetValueAtLevel(SubJobLevel) : 0,
		SubJobEfficiency);
	
	Mind = CalculateValue(bRaceValid ? Race->BaseAttributes.Mind.GetValueAtLevel(Level) : 1,
		bMainJobValid ? MainJob->BaseAttributes.Mind.GetValueAtLevel(MainJobLevel) : 1,
		bSubJobValid ? SubJob->BaseAttributes.Mind.GetValueAtLevel(SubJobLevel) : 0,
		SubJobEfficiency);
	
	Charisma = CalculateValue(bRaceValid ? Race->BaseAttributes.Charisma.GetValueAtLevel(Level) : 1,
		bMainJobValid ? MainJob->BaseAttributes.Charisma.GetValueAtLevel(MainJobLevel) : 1,
		bSubJobValid ? SubJob->BaseAttributes.Charisma.GetValueAtLevel(SubJobLevel) : 0,
		SubJobEfficiency);
}

int32 FPrimaryAttributesCalc::CalculateValue(const float Race, const float MainJob, 
                                             const float SubJob, const float SubJobEfficiency)
{
	return FMath::Floor(Race * (MainJob + (SubJob * SubJobEfficiency)));
}

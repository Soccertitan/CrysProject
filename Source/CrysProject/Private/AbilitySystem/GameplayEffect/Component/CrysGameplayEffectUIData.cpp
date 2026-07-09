// Copyright Soccertitan 2026


#include "AbilitySystem/GameplayEffect/Component/CrysGameplayEffectUIData.h"

void UCrysGameplayEffectUIData::OnGameplayEffectChanged()
{
	Super::OnGameplayEffectChanged();
	
	for (TInstancedStruct<FCrysGameplayEffectTextBase>& GameplayEffectText : GameplayEffectTexts)
	{
		if (FCrysGameplayEffectTextBase* GameplayEffectTextPtr = GameplayEffectText.GetMutablePtr())
		{
			GameplayEffectTextPtr->OnGameplayEffectChanged(GetOwner());
		}
	}
	
	TextPreview = GenerateText(0); 
}

FText UCrysGameplayEffectUIData::GenerateText(float Level) const
{
	FFormatOrderedArguments Args;
	for (const TInstancedStruct<FCrysGameplayEffectTextBase>& GameplayEffectText : GameplayEffectTexts)
	{
		if (const FCrysGameplayEffectTextBase* GameplayEffectTextPtr = GameplayEffectText.GetPtr())
		{
			Args.Add(GameplayEffectTextPtr->GenerateText(GetOwner(), Level));
		}
	}
	return FText::Format(TextFormat, Args);
}

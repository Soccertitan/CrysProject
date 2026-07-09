// Copyright Soccertitan 2026


#include "AbilitySystem/GameplayEffect/Component/GameplayEffectTextAttributes.h"

#include "GameplayEffect.h"
#include "AbilitySystem/CrysAbilityBlueprintFunctionLibrary.h"
#include "AbilitySystem/GameplayTagInfoFragment_Attribute.h"
#include "Settings/CrysGameData.h"
#include "System/CrysAssetManager.h"
#include "System/CrysGameplayTagRelationship.h"


#define LOCTEXT_NAMESPACE "CrysGamepalyEffectText"

FTextFormat FGameplayEffectAttributeText::TextFormat = LOCTEXT("AttributeText", "{DisplayText}{Modifier}{Value}");
FTextFormat FGameplayEffectTextAttributes::TextFormat = LOCTEXT("GeneratedAttributeText", "{Start} {Append}");

FText FGameplayEffectAttributeText::GenerateText(float Value) const
{
	FFormatNamedArguments Args;
	Args.Add(TEXT("DisplayText"), DisplayText);
	Args.Add(TEXT("Value"), Value);
	
	FText ModifierOpChecked = ModifierOpText;
	if (ModifierOp == EGameplayModOp::AddBase || ModifierOp == EGameplayModOp::AddFinal)
	{
		ModifierOpChecked = Value > 0.f ? ModifierOpText : FText();
		
		if (bDisplayValueAsPercent)
		{
			Args.Add(TEXT("Value"), FText::AsPercent(Value));
		}
	}
	else if (ModifierOp == EGameplayModOp::Division || ModifierOp == EGameplayModOp::DivideAdditive)
	{
		Args.Add(TEXT("Value"), FMath::SafeDivide(1, Value));
	}
	Args.Add(TEXT("Modifier"), ModifierOpChecked);
	
	return FText::Format(TextFormat , Args);
}

void FGameplayEffectTextAttributes::OnGameplayEffectChanged(UGameplayEffect* Owner)
{
	const UCrysGameplayTagRelationship* GameplayTagRelationship = UCrysAssetManager::GetAsset(GetDefault<UCrysGameData>()->GameplayTagRelationship, false);
	
	if (!GameplayTagRelationship)
	{
		return;
	}
	
	AttributeTexts.Reset();
	for (int32 Index = 0; Index < Owner->Modifiers.Num(); Index++)
	{
		if (ModifiersToIgnore.Contains(Index))
		{
			continue;
		}
		const FGameplayModifierInfo& Modifier = Owner->Modifiers[Index];
		
		const FCrysGameplayTagInfo* GameplayTagInfo = UCrysAbilityBlueprintFunctionLibrary::FindAttributeGameplayTagInfo(GameplayTagRelationship, Modifier.Attribute, Modifier.SourceTags.RequireTags, false);
		if (GameplayTagInfo)
		{
			FGameplayEffectAttributeText& AttributeText = AttributeTexts.AddDefaulted_GetRef();
			AttributeText.ModifierOp = Modifier.ModifierOp;
			AttributeText.ModifierIndex = Index;
			AttributeText.ModifierOpText = GetModifierOpText(Modifier.ModifierOp);
			
			const FGameplayTagInfoFragment_Attribute* Fragment_Attribute = GameplayTagInfo->FindFragmentByType<FGameplayTagInfoFragment_Attribute>();
			AttributeText.DisplayText = Fragment_Attribute->DisplayText;
			AttributeText.bDisplayValueAsPercent = Fragment_Attribute->bDisplayValueAsPercent;
		}
	}
}

FText FGameplayEffectTextAttributes::GenerateText(UGameplayEffect* Owner, float Level) const
{
	FText Result;
	for (int32 Index = 0; Index < AttributeTexts.Num(); Index++)
	{
		const FGameplayEffectAttributeText& AttributeText = AttributeTexts[Index];
		float AttributeValue = 0.f;
		const bool bStaticFound = Owner->Modifiers[AttributeText.ModifierIndex].ModifierMagnitude.GetStaticMagnitudeIfPossible(Level, AttributeValue);
		if (bStaticFound)
		{
			const FText GeneratedText = AttributeText.GenerateText(AttributeValue);
			if (Result.IsEmpty())
			{
				Result = GeneratedText;
			}
			else
			{
				FFormatNamedArguments Args;
				Args.Add(TEXT("Start"), Result);
				Args.Add(TEXT("Append"), GeneratedText);
				
				Result = FText::Format(TextFormat, Args);
			}
		}
	}
	return Result;
}

FText FGameplayEffectTextAttributes::GetModifierOpText(TEnumAsByte<EGameplayModOp::Type> ModifierOp)
{
	if (ModifierOp == EGameplayModOp::AddBase || ModifierOp == EGameplayModOp::AddFinal)
	{
		return LOCTEXT("Addition", "+");
	}
	
	if (ModifierOp == EGameplayModOp::Multiplicitive || ModifierOp == EGameplayModOp::MultiplyAdditive || ModifierOp == EGameplayModOp::MultiplyCompound ||
		ModifierOp == EGameplayModOp::Division || ModifierOp == EGameplayModOp::DivideAdditive)
	{
		return LOCTEXT("Multiply", "*");
	}
	
	return LOCTEXT("Override", "=");
}

#undef LOCTEXT_NAMESPACE

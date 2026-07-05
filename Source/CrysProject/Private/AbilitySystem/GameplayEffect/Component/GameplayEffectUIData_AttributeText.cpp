// Copyright Soccertitan 2026


#include "AbilitySystem/GameplayEffect/Component/GameplayEffectUIData_AttributeText.h"

#include "GameplayEffect.h"
#include "VREditorMode.h"
#include "AbilitySystem/AttributeTagRelationship.h"
#include "Settings/CrysGameData.h"
#include "System/CrysAssetManager.h"
#include "UI/UITagRelationship.h"

#define LOCTEXT_NAMESPACE "GameplayEffectUIData_AttributeText"

FTextFormat FGameplayEffectUIDataAttributeText::TextFormat = LOCTEXT("AttributeText", "{ShortName}{Modifier}{Value}");
FTextFormat UGameplayEffectUIData_AttributeText::TextFormat = LOCTEXT("GeneratedAttributeText", "{Start} {Append}");

FText FGameplayEffectUIDataAttributeText::GenerateText(float Value) const
{
	FFormatNamedArguments Args;
	Args.Add(TEXT("ShortName"), ShortName);
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

FText UGameplayEffectUIData_AttributeText::GetModifierOpText(TEnumAsByte<EGameplayModOp::Type> ModifierOp)
{
	if (ModifierOp == EGameplayModOp::AddBase || ModifierOp == EGameplayModOp::AddFinal)
	{
		return LOCTEXT("Addition", "+");
	}
	
	if (ModifierOp == EGameplayModOp::Multiplicitive || ModifierOp == EGameplayModOp::MultiplyAdditive || ModifierOp == EGameplayModOp::MultiplyCompound)
	{
		return LOCTEXT("Multiply", "*");
	}
	
	if (ModifierOp == EGameplayModOp::Division || ModifierOp == EGameplayModOp::DivideAdditive)
	{
		return LOCTEXT("Multiply", "*");
	}
	
	return LOCTEXT("Override", "=");
}

FText UGameplayEffectUIData_AttributeText::GetAttributeDescription(const float Level) const
{
	FText Result;
	for (int32 Index = 0; Index < AttributeTexts.Num(); Index++)
	{
		const FGameplayEffectUIDataAttributeText& AttributeText = AttributeTexts[Index];
		float AttributeValue = 0.f;
		const bool bStaticFound = GetOwner()->Modifiers[Index].ModifierMagnitude.GetStaticMagnitudeIfPossible(Level, AttributeValue);
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

void UGameplayEffectUIData_AttributeText::OnGameplayEffectChanged()
{
	Super::OnGameplayEffectChanged();
	
	const UUITagRelationship* UITagRelationship = UCrysAssetManager::GetAsset(GetDefault<UCrysGameData>()->UITagRelationship);
	const UAttributeTagRelationship* AttributeTagRelationship = UCrysAssetManager::GetAsset(GetDefault<UCrysGameData>()->AttributeTagRelationship);
	
	if (!UITagRelationship || !AttributeTagRelationship)
	{
		return;
	}
	
	AttributeTexts.Reset(GetOwner()->Modifiers.Num());
	for (const FGameplayModifierInfo& Modifier : GetOwner()->Modifiers)
	{
		FGameplayEffectUIDataAttributeText& AttributeText = AttributeTexts.AddDefaulted_GetRef();
		AttributeText.ModifierOp = Modifier.ModifierOp;
		AttributeText.ModifierOpText = GetModifierOpText(Modifier.ModifierOp);
		
		FAttributeTagInfo AttributeTagInfo = AttributeTagRelationship->FindAttributeTagInfo(Modifier.Attribute, Modifier.SourceTags.RequireTags, true);
		if (AttributeTagInfo.IsValid())
		{
			AttributeText.ShortName = UITagRelationship->FindUITagInfo(AttributeTagInfo.AttributeTag, true).ShortName;
			AttributeText.bDisplayValueAsPercent = AttributeTagInfo.bDisplayValueAsPercent;
		}
	}
}

#undef LOCTEXT_NAMESPACE

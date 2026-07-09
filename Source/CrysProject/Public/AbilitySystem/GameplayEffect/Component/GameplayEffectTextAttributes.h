// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "CrysGameplayEffectUIData.h"
#include "GameplayEffectTypes.h"
#include "GameplayEffectTextAttributes.generated.h"


USTRUCT()
struct FGameplayEffectAttributeText
{
	GENERATED_BODY()
	
	FText GenerateText(float Value) const;
	
	FText DisplayText = FText();
	FText ModifierOpText = FText();
	int32 ModifierIndex = -1;
	
	TEnumAsByte<EGameplayModOp::Type> ModifierOp;
	bool bDisplayValueAsPercent = false;
	
	static FTextFormat TextFormat;
};

/** Generates text for all AttributeModifiers. */
USTRUCT(DisplayName = "Attributes")
struct FGameplayEffectTextAttributes : public FCrysGameplayEffectTextBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, meta = (ClampMin = 0))
	TArray<int32> ModifiersToIgnore;
	
	virtual void OnGameplayEffectChanged(UGameplayEffect* Owner) override;
	virtual FText GenerateText(UGameplayEffect* Owner, float Level) const override;
	
	static FText GetModifierOpText(TEnumAsByte<EGameplayModOp::Type> ModifierOp);
	static FTextFormat TextFormat;
	
private:
	TArray<FGameplayEffectAttributeText> AttributeTexts;
};

// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameplayEffectUIData.h"
#include "GameplayEffectUIData_AttributeText.generated.h"


USTRUCT()
struct FGameplayEffectUIDataAttributeText
{
	GENERATED_BODY()
	
	FText GenerateText(float Value) const;
	
	UPROPERTY(VisibleAnywhere)
	FText ShortName = FText();
	UPROPERTY(VisibleAnywhere)
	FText ModifierOpText = FText();
	
	TEnumAsByte<EGameplayModOp::Type> ModifierOp;
	bool bDisplayValueAsPercent = false;
};

/**
 * Automatically gets all the modifiers for the GE and adds it to an array for displaying in UI.
 */
UCLASS(DisplayName="UI Data (Attribute Text)")
class CRYSPROJECT_API UGameplayEffectUIData_AttributeText : public UGameplayEffectUIData
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure)
	virtual FText GetAttributeDescription(const float Level) const;
	
	virtual void OnGameplayEffectChanged() override;
	
	static FText GetModifierOpText(TEnumAsByte<EGameplayModOp::Type> ModifierOp);
	
private:
	UPROPERTY(VisibleAnywhere)
	TArray<FGameplayEffectUIDataAttributeText> AttributeTexts;
};

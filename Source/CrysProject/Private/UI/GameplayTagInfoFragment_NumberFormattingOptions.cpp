// Copyright Soccertitan 2026


#include "UI/GameplayTagInfoFragment_NumberFormattingOptions.h"


FText FGameplayTagInfoFragment_NumberFormatingOptions::GetText(float Value) const
{
	FNumberFormattingOptions FormattingOptions;
	FormattingOptions.AlwaysSign = bAlwaysSign;
	FormattingOptions.UseGrouping = bUseGrouping;
	FormattingOptions.MinimumIntegralDigits = MinimumIntegralDigits;
	FormattingOptions.MaximumIntegralDigits = MaximumIntegralDigits;
	FormattingOptions.MinimumFractionalDigits = MinimumFractionalDigits;
	FormattingOptions.MaximumFractionalDigits = MaximumFractionalDigits;
	FormattingOptions.RoundingMode = RoundingMode;
	
	if (bDisplayValueAsPercent)
	{
		return FText::AsPercent(Value, &FormattingOptions);
	}
	
	return FText::AsNumber(Value, &FormattingOptions);
}

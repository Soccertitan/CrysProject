// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "System/CrysGameplayTagRelationship.h"
#include "GameplayTagInfoFragment_NumberFormattingOptions.generated.h"

/**
 * Formatting options to convert a number to text.
 */
USTRUCT(DisplayName = "NumberFormatingOptions")
struct FGameplayTagInfoFragment_NumberFormatingOptions : public FCrysGameplayTagInfoFragment
{
	GENERATED_BODY()
	
	/** Set to true to display the value as a percent. i.e. 0.12 will display as 12% */
	UPROPERTY(EditAnywhere)
	bool bDisplayValueAsPercent = false;
	
	UPROPERTY(EditAnywhere)
	bool bAlwaysSign = false;
	
	UPROPERTY(EditAnywhere)
	bool bUseGrouping = true;
	
	UPROPERTY(EditAnywhere, meta = (ClampMin = 1))
	int32 MinimumIntegralDigits = 1;
	
	UPROPERTY(EditAnywhere, meta = (ClampMin = 1, ClampMax = 324))
	int32 MaximumIntegralDigits = 324;
	
	UPROPERTY(EditAnywhere, meta = (ClampMin = 0))
	int32 MinimumFractionalDigits = 0;
	
	UPROPERTY(EditAnywhere, meta = (ClampMin = 0, ClampMax = 3))
	int32 MaximumFractionalDigits = 0;
	
	UPROPERTY(EditAnywhere)
	TEnumAsByte<ERoundingMode> RoundingMode = ToZero;
	
	FText GetText(float Value) const;
};

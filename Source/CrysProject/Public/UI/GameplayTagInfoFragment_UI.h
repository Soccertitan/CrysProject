// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "System/CrysGameplayTagRelationship.h"

#include "GameplayTagInfoFragment_UI.generated.h"

USTRUCT(DisplayName = "UI")
struct FGameplayTagInfoFragment_UI : public FCrysGameplayTagInfoFragment
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	FText Name;

	UPROPERTY(EditAnywhere)
	FText ShortName;
	
	UPROPERTY(EditAnywhere, meta = (MultiLine=true))
	FText Description;
	
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UTexture2D> Icon;
};

// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectUIData.h"
#include "StructUtils/InstancedStruct.h"
#include "CrysGameplayEffectUIData.generated.h"


/** A generic struct used in the CrysGameplayEffectUIData to generate text for a GE dynamically. */
USTRUCT()
struct FCrysGameplayEffectTextBase
{
	GENERATED_BODY()
	
	FCrysGameplayEffectTextBase(){}
	virtual ~FCrysGameplayEffectTextBase() {}

	virtual FText GenerateText(UGameplayEffect* Owner, float Level) const {return FText();}
	virtual void OnGameplayEffectChanged(UGameplayEffect* Owner) {}
};

/**
 * Displays text in a dynamic way.
 */
UCLASS(DisplayName="Crys UI Data")
class CRYSPROJECT_API UCrysGameplayEffectUIData : public UGameplayEffectUIData
{
	GENERATED_BODY()
	
public:
	virtual void OnGameplayEffectChanged() override;
	
	FText GenerateText(float Level) const;
	
protected:
	UPROPERTY(VisibleAnywhere)
	FText TextPreview;
	
	/** Use the special characters {0}...{n} to have the GameplayEffectTexts insert text into the format as an ordered array. */
	UPROPERTY(EditAnywhere, meta = (MultiLine))
	FText TextFormat;
	
	UPROPERTY(EditAnywhere, meta = (ExcludeBaseStruct))
	TArray<TInstancedStruct<FCrysGameplayEffectTextBase>> GameplayEffectTexts;
};

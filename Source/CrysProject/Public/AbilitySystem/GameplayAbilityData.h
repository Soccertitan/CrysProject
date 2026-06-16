// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayAbilityData.generated.h"

class UAbilityInputSlotUserWidget;
class UCrimGameplayAbility;
class UAbilityViewModel;

/**
 * Contains data for abilities in this project.
 */
UCLASS(Const)
class CRYSPROJECT_API UGameplayAbilityData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UGameplayAbilityData();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	TSoftClassPtr<UCrimGameplayAbility> AbilityClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	FText AbilityName;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSoftObjectPtr<UTexture2D> Icon;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI", NoClear)
	TSubclassOf<UAbilityViewModel> AbilityViewModel;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSoftClassPtr<UAbilityInputSlotUserWidget> AbilityInputSlotUserWidgetClass;
};

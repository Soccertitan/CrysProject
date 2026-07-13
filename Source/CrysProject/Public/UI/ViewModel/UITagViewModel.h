// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "System/CrysGameplayTagRelationship.h"
#include "UI/GameplayTagInfoFragment_UI.h"
#include "UITagViewModel.generated.h"


class UTexture2D;

/**
 * Display information on Gameplay Tags.
 */
UCLASS()
class CRYSPROJECT_API UUITagViewModel : public UMVVMViewModelBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Viewmodel|UI")
	void SetGameplayTag(const FGameplayTag& InValue);
	
	UFUNCTION(BlueprintPure, FieldNotify, Category = "Viewmodel|UI")
	FText GetTagName() const;
	UFUNCTION(BlueprintPure, FieldNotify, Category = "Viewmodel|UI")
	FText GetShortName() const;
	UFUNCTION(BlueprintPure, FieldNotify, Category = "Viewmodel|UI")
	FText GetDescription() const;
	UFUNCTION(BlueprintPure, FieldNotify, Category = "Viewmodel|UI")
	TSoftObjectPtr<UTexture2D> GetIcon() const;

private:
	FGameplayTag GameplayTag;
	
	FGameplayTagInfoFragment_UI Fragment_UI;
};

// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "UI/UITagRelationship.h"
#include "UITagViewModel.generated.h"

/**
 * Display information on Gameplay Tags.
 */
UCLASS()
class CRYSPROJECT_API UUITagViewModel : public UMVVMViewModelBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Viewmodel|GameplayTag")
	void SetGameplayTag(const FGameplayTag& InValue);
	
	UFUNCTION(BlueprintPure, FieldNotify)
	FText GetTagName() const {return UITagInfo.Name;}
	UFUNCTION(BlueprintPure, FieldNotify)
	FText GetShortName() const {return UITagInfo.ShortName;}
	UFUNCTION(BlueprintPure, FieldNotify)
	FText GetDescription() const {return UITagInfo.Description;}
	UFUNCTION(BlueprintPure, FieldNotify)
	TSoftObjectPtr<UTexture2D> GetIcon() const {return UITagInfo.Icon;}
	
private:
	UPROPERTY()
	FUITagInfo UITagInfo;
};

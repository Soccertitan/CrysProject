// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "AbilityViewModel.generated.h"

/**
 * Details about an ability.
 */
UCLASS()
class CRYSPROJECT_API UAbilityViewModel : public UMVVMViewModelBase
{
	GENERATED_BODY()
	
public:
	FText GetAbilityName() const { return AbilityName; }
	TSoftObjectPtr<UTexture2D> GetIcon() const { return Icon; }
	
protected:
	void SetAbilityName(const FText& NewValue);
	void SetIcon(const TSoftObjectPtr<UTexture2D>& NewValue);
	
	//TODO: Add virtual func to set the AbilityData.
	
private:
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Category = "Viewmodel|Ability", meta = (AllowPrivateAccess = true))
	FText AbilityName;
	
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Category = "Viewmodel|Ability", meta = (AllowPrivateAccess = true))
	TSoftObjectPtr<UTexture2D> Icon;
	
	//TODO: Add property for the actual Ability data.
};

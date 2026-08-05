// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "ActivatableAbilitiesViewModel.generated.h"

struct FGameplayAbilitySpec;
class UCrimAbilitySystemComponent;
class UCrysAbilityViewModel;

/**
 * Generates AbilityViewModels of all activatable abilities.
 */
UCLASS()
class CRYSPROJECT_API UActivatableAbilitiesViewModel : public UMVVMViewModelBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, FieldNotify)
	TArray<UCrysAbilityViewModel*> GetAbilityViewModels() const { return AbilityViewModels; }
	
	void SetAbilitySystemComponent(UCrimAbilitySystemComponent* ASC);
	
private:
	UPROPERTY()
	TObjectPtr<UCrimAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY()
	TArray<TObjectPtr<UCrysAbilityViewModel>> AbilityViewModels;
	
	void OnAbilityGiven(const FGameplayAbilitySpec& Spec);
	void OnAbilityRemoved(const FGameplayAbilitySpec& Spec);
	
	bool TryCreateViewModel(const FGameplayAbilitySpec& Spec);
};

// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "UI/ViewModel/CrysViewModel.h"
#include "PlayerAttributesViewModel.generated.h"

struct FGameplayTag;
class UAttributeFractionViewModel;
class UAttributeViewModel;
class UCrimAbilitySystemComponent;

/**
 * Caches the players ASC and can dynamic add ViewModels based on AttributeTags.
 */
UCLASS()
class CRYSPROJECT_API UPlayerAttributesViewModel : public UCrysViewModel
{
	GENERATED_BODY()
	
public:
	virtual void InitializeViewModel(APlayerController* PlayerController) override;
	
	/** Finds an existing ViewModel or initializes a new one with the cached ASC. */
	UFUNCTION(BlueprintCallable, Category = "Viewmodel|Attribute")
	UAttributeViewModel* FindOrCreateAttributeViewModel(const FGameplayTag& AttributeTag);
	
	/** Finds an existing ViewModel or initializes a new one with the cached ASC. */
	UFUNCTION(BlueprintCallable, Category = "Viewmodel|Attribute")
	UAttributeFractionViewModel* FindOrCreateAttributeFractionViewModel(const FGameplayTag& DividendAttributeTag, const FGameplayTag DivisorAttributeTag);
	
protected:
	UFUNCTION(BlueprintPure, Category = "Viewmodel|Attribute")
	UCrimAbilitySystemComponent* GetAbilitySystemComponent() const { return AbilitySystemComponent; }
	
private:
	/** Cached ASC. */
	UPROPERTY()
	TObjectPtr<UCrimAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY()
	TArray<TWeakObjectPtr<UAttributeViewModel>> AttributeViewModels;
	UPROPERTY()
	TArray<TWeakObjectPtr<UAttributeFractionViewModel>> AttributeFractionViewModels;
};

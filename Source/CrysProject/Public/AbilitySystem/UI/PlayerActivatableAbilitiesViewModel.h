// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "UI/ViewModel/CrysViewModel.h"
#include "PlayerActivatableAbilitiesViewModel.generated.h"

class UActivatableAbilitiesViewModel;
class UCrimAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class CRYSPROJECT_API UPlayerActivatableAbilitiesViewModel : public UCrysViewModel
{
	GENERATED_BODY()
	
public:
	virtual void InitializeViewModel(APlayerController* PlayerController) override;
	
	UActivatableAbilitiesViewModel* GetActivatableAbilitiesViewModel() const { return ActivatableAbilitiesViewModel; }
	
protected:
	UFUNCTION(BlueprintPure, Category = "Viewmodel|Attribute")
	UCrimAbilitySystemComponent* GetAbilitySystemComponent() const { return AbilitySystemComponent; }
	
private:
	/** Cached ASC. */
	UPROPERTY()
	TObjectPtr<UCrimAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, meta = (AllowPrivateAccess = true))
	TObjectPtr<UActivatableAbilitiesViewModel> ActivatableAbilitiesViewModel;
};

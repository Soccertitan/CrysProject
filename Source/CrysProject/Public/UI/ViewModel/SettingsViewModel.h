// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "CrysViewModel.h"
#include "Input/AbilityInputMode.h"
#include "SettingsViewModel.generated.h"

/**
 * A view model for modifying and saving settings.
 */
UCLASS()
class CRYSPROJECT_API USettingsViewModel : public UCrysViewModel
{
	GENERATED_BODY()
	
public:
	virtual void InitializeViewModel(APlayerController* PlayerController) override;
	
	EAbilityInputMode GetAbilityInputMode() const { return AbilityInputMode; }
	/** Sets the input mode on the Hero. */
	UFUNCTION(BlueprintCallable, Category = "Viewmodel|Settings", DisplayName = "SetAbilityInputMode")
	void K2_SetAbilityInputMode(const EAbilityInputMode NewValue);
	
protected:
	UFUNCTION()
	void SetAbilityInputMode(const EAbilityInputMode NewValue);
	
private:
	UPROPERTY()
	APlayerController* OwnerPC;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, meta = (AllowPrivateAccess = true))
	EAbilityInputMode AbilityInputMode;
};

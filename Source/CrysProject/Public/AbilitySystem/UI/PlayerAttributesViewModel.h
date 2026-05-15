// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "UI/ViewModel/CrysViewModel.h"
#include "PlayerAttributesViewModel.generated.h"

class UCrimAbilitySystemComponent;

/**
 * Designed to be overriden to show specific attributes for the hero.
 */
UCLASS(Abstract)
class CRYSPROJECT_API UPlayerAttributesViewModel : public UCrysViewModel
{
	GENERATED_BODY()
	
public:
	virtual void InitializeViewModel(APlayerController* PlayerController) override;
	
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void OnAbilitySystemSet(UCrimAbilitySystemComponent* AbilitySystemComponent);
};

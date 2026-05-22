// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "CrysPlayerController.h"
#include "HeroPlayerController.generated.h"

class ULockOnSystemComponent;
class UCrimAbilitySystemComponent;
class UAbilityInputManagerComponent;

/**
 * 
 */
UCLASS()
class CRYSPROJECT_API AHeroPlayerController : public ACrysPlayerController
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilityInputManager", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAbilityInputManagerComponent> AbilityInputManagerComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LockOnSystem", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ULockOnSystemComponent> LockOnSystemComponent;
	
public:
	AHeroPlayerController();

	//-----------------------------------------------------------------------------------------
	// Class overrides
	//-----------------------------------------------------------------------------------------
	virtual void AcknowledgePossession(class APawn* P) override;
	virtual void OnRep_PlayerState() override;
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;
	
protected:
	virtual void OnPossess(APawn* InPawn) override;

private:
	/** Cached ASC from the PlayerState. */
	UPROPERTY()
	TObjectPtr<UCrimAbilitySystemComponent> AbilitySystemComponent;
	
	/**
	 *  Retrieve the Ability System Component from the PlayerState and caches it locally. And initializing
	 *  other components that need a reference to the ASC.
	 */
	void InitAbilitySystemComponent();
};

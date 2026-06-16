// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "CrimTargetingSystemInterface.h"
#include "CrysPlayerController.h"
#include "Input/AbilityInputMode.h"
#include "HeroPlayerController.generated.h"

class UCrimTargetingSystemComponent;
class UCrimAbilitySystemComponent;
class UAbilityInputManagerComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHeroPlayerControllerInputModeSignature, EAbilityInputMode, InputMode);

/**
 * 
 */
UCLASS()
class CRYSPROJECT_API AHeroPlayerController : public ACrysPlayerController, public ICrimTargetingSystemInterface
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilityInputManager", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAbilityInputManagerComponent> AbilityInputManagerComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerLockOn", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCrimTargetingSystemComponent> TargetingSystemComponent;
	
public:
	AHeroPlayerController();
	
	// CrimTargetingSystemInterface
	virtual UCrimTargetingSystemComponent* GetCrimTargetingSystemComponent_Implementation() const override;
	
	UPROPERTY(BlueprintAssignable, DisplayName = "OnAbilityInputModeChanged")
	FHeroPlayerControllerInputModeSignature OnAbilityInputModeChangedDelegate;
	
	UFUNCTION(BlueprintPure, Category = "PlayerController")
	EAbilityInputMode GetAbilityInputMode() const { return AbilityInputMode; }
	UFUNCTION(BlueprintCallable, Category = "PlayerController")
	void SetAbilityInputMode(const EAbilityInputMode InputMode);

	//-----------------------------------------------------------------------------------------
	// Class overrides
	//-----------------------------------------------------------------------------------------
	virtual void AcknowledgePossession(class APawn* P) override;
	virtual void OnRep_PlayerState() override;
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;
	
protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void SetupInputComponent() override;

private:
	/** Cached ASC from the PlayerState. */
	UPROPERTY()
	TObjectPtr<UCrimAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	EAbilityInputMode AbilityInputMode = EAbilityInputMode::Keyboard;
	UPROPERTY(EditAnywhere, Category = "Input")
	TMap<EAbilityInputMode, TObjectPtr<UInputMappingContext>> AbilityInputModeInputMappingContextMap;
	UPROPERTY(EditAnywhere, Category = "Input")
	int32 AbilityInputMappingContextPriority = 0;
	
	/**
	 *  Retrieve the Ability System Component from the PlayerState and caches it locally. And initializing
	 *  other components that need a reference to the ASC.
	 */
	void InitAbilitySystemComponent();
	
	/** Will set the InputMode as specified and updates the input mapping context. */
	void InternalSetAbilityInputMode(const EAbilityInputMode InputMode);
};

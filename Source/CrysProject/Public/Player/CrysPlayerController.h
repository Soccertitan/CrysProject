// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UINavPCReceiver.h"
#include "GameFramework/PlayerController.h"
#include "CrysPlayerController.generated.h"

class UInputActionListenerMap;
class UInputMappingContext;
class UWidget;
class UUINavPCComponent;
class UEnhancedInputLocalPlayerSubsystem;
DECLARE_MULTICAST_DELEGATE(FCrysPlayerControllerGenericSignature);

/**
 * The base player controller used in this project.
 */
UCLASS()
class CRYSPROJECT_API ACrysPlayerController : public APlayerController, public IUINavPCReceiver
{
	GENERATED_BODY()
	
public:
	ACrysPlayerController();
	virtual void OnRep_PlayerState() override;

	/**
	 * Creates a widget and then adds it to the Viewport. Handles setting up the input and focus.
	 * @param WidgetClass The WidgetClass to create and add to the Viewport.
	 * @param bRestrictMovement If true, will disable move and look input.
	 * @param bIsPushed If true, adds the widget to the top of the stack. If false, removes all widgets before adding.
	 */
	UFUNCTION(BlueprintCallable, Category = "PlayerController", meta = (DeterminesOutputType = "WidgetClass", AdvancedDisplay = 2))
	UUINavWidget* CreateUINavWidgetAndAddToViewport(const TSubclassOf<UUINavWidget> WidgetClass, const bool bRestrictMovement = true, const bool bIsPushed = true, const bool bRemoveParent = false, const bool bDestroyParent = false, const int ZOrder = 0);

	/**
	 * Adds the specified UINavWidget to the viewport. Handles setting up the input mode and focus.
	 * @param InWidget The UINav Widget to add to the viewport.
	 * @param InWidgetToFocus The widget to focus on.
	 * @param bRestrictMovement If true, will disable move and look input.
	 * @param bIsPushed If true, adds the widget to the top of the stack. If false, removes all widgets before adding.
	 */
	UFUNCTION(BlueprintCallable, Category = "PlayerController", meta = (AdvancedDisplay = 3))
	void AddUINavWidgetToViewport(UUINavWidget* InWidget, UWidget* InWidgetToFocus = nullptr, const bool bRestrictMovement = true, const bool bIsPushed = true, const bool bRemoveParent = false, const bool bDestroyParent = false, const int ZOrder = 0);
	
	FCrysPlayerControllerGenericSignature OnRootWidgetAddedDelegate;
	FCrysPlayerControllerGenericSignature OnRootWidgetRemovedDelegate;
	
	// UINavPCReceiver Interface
	virtual void OnRootWidgetAdded_Implementation() override;
	virtual void OnRootWidgetRemoved_Implementation() override;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UINavPCComponent);
	TObjectPtr<UUINavPCComponent> UINavPCComponent;
	UPROPERTY()
	TObjectPtr<UEnhancedInputLocalPlayerSubsystem> EnhancedInputSubsystem;
	
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	/** Restricts movement and look input when a UINavWidget is added to the viewport. */
	bool bRestrictMovementInUI = false;
	
	/** The InputMappingContexts to apply at startup with the integer as the priority. */
	UPROPERTY(EditAnywhere, Category = "Input", meta = (NoResetToDefault))
	TMap<TObjectPtr<UInputMappingContext>, int32> InputMappingContextMap;
	/** Binds the Listeners to the InputActions during SetupInputComponent. */
	UPROPERTY(EditAnywhere, Category = "Input", meta = (NoResetToDefault))
	TArray<TObjectPtr<UInputActionListenerMap>> StartupInputActionListeners;
	
	/** Adds the Inputs in the StartupInputActionListeners to the CrimEnhancedInputComponent. */
	void ApplyStartupInputActionMap();

	/** Initializes the CrysHUD when a valid PlayerState has been received. */
	void TryInitializeHUD();
	bool bHudInitialized = false;
};

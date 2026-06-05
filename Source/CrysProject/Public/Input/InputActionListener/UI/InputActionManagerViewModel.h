// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "UI/ViewModel/CrysViewModel.h"
#include "InputActionManagerViewModel.generated.h"

struct FCrimEnhancedInputActionBinding;
class UInputAction;
class UCrimEnhancedInputComponent;
class UInputActionListenerViewModel;

/**
 * Gets the InputActionListeners from the player.
 */
UCLASS()
class CRYSPROJECT_API UInputActionManagerViewModel : public UCrysViewModel
{
	GENERATED_BODY()
	
public:
	virtual void InitializeViewModel(APlayerController* PlayerController) override;
	
	/** Finds or creates an InputActionListener VM from an InputAction. */
	UFUNCTION(BlueprintCallable, Category = "Viewmodel|InputAction")
	UInputActionListenerViewModel* FindOrCreateInputActionListenerViewModel(UInputAction* InputAction);
	
protected:
	void InitEnhancedInputComponent(APlayerController* PlayerController);
	
private:
	UPROPERTY()
	TObjectPtr<UCrimEnhancedInputComponent> EnhancedInputComponent;
	
	UPROPERTY()
	TArray<TObjectPtr<UInputActionListenerViewModel>> InputActionListenerViewModels;
	
	UInputActionListenerViewModel* CreateInputActionListenerViewModelInternal(UInputAction* InputAction);
	
	UFUNCTION()
	void OnInputActionListenerAdded(const FCrimEnhancedInputActionBinding& InputActionBinding);
	
	UFUNCTION()
	void OnInputActionListenerRemoved(const FCrimEnhancedInputActionBinding& InputActionBinding);
};

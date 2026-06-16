// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "InputActionListenerViewModel.generated.h"

class UCrysInputActionListener;
class UInputAction;
/**
 * A generic ViewModel that broadcasts when the IA is pressed/released.
 */
UCLASS()
class CRYSPROJECT_API UInputActionListenerViewModel : public UMVVMViewModelBase
{
	GENERATED_BODY()
	
public:
	void InitializeInputActionListener(UInputAction* InInputAction, UCrysInputActionListener* InInputActionListener);
	UInputAction* GetInputAction() const { return InputAction; }
	UCrysInputActionListener* GetInputActionListener() const {return InputActionListener;}
	
	bool IsPressed() const { return bPressed; }
	
protected:
	void SetInputActionListener(UCrysInputActionListener* InInputActionListener);
	void SetIsPressed(bool InValue);

private:
	UPROPERTY()
	TObjectPtr<UInputAction> InputAction;
	UPROPERTY()
	TObjectPtr<UCrysInputActionListener> InputActionListener;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, FieldNotify, Getter=IsPressed, meta = (AllowPrivateAccess = true))
	bool bPressed = false;
	
	UFUNCTION()
	void OnInputPressed(UCrysInputActionListener* Action);
	UFUNCTION()
	void OnInputReleased(UCrysInputActionListener* Action);
	
	friend class UInputActionManagerViewModel;
};

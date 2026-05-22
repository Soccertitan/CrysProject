// Copyright Soccertitan 2025


#include "Input/InputActionListener/InputActionListener_StopAttackClearTarget.h"

#include "InputActionValue.h"


void UInputActionListener_StopAttackClearTarget::OnInitializeListener()
{
	Super::OnInitializeListener();
	
	// GetAutoAttackManagerComponent();
}

void UInputActionListener_StopAttackClearTarget::OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn)
{
	Super::OnPossessedPawnChanged(OldPawn, NewPawn);
	
	// TargetingSystemComponent = UTargetingSystemBlueprintFunctionLibrary::GetTargetingSystemComponent(NewPawn);
}

void UInputActionListener_StopAttackClearTarget::OnInputActionTriggered(const FInputActionValue& Value)
{
	Super::OnInputActionTriggered(Value);
	
	if (Value.Get<bool>() == false)
	{
		return;
	}
	
	// if (GetAutoAttackManagerComponent() && AutoAttackManagerComponent->IsAutoAttacking())
	// {
	// 	AutoAttackManagerComponent->StopAutoAttack();
	// 	return;
	// }
	//
	// if (TargetingSystemComponent)
	// {
	// 	if (TargetingSystemComponent->IsCameraLocked())
	// 	{
	// 		TargetingSystemComponent->SetCameraLock(false);
	// 		return;
	// 	}
	// 	if (TargetingSystemComponent->GetTargetedPoint())
	// 	{
	// 		TargetingSystemComponent->ClearTarget();
	// 		return;
	// 	}
	// }
}

// UAutoAttackManagerComponent* UStopAttackClearTargetInputActionListener::GetAutoAttackManagerComponent()
// {
	// if (!AutoAttackManagerComponent)
	// {
	// 	if (GetPlayerController() && GetPlayerController()->GetPlayerState<APlayerState>())
	// 	{
	// 		// AutoAttackManagerComponent = GetPlayerController()->GetPlayerState<APlayerState>()->FindComponentByClass<UAutoAttackManagerComponent>();
	// 	}
	// }
	//
	// return AutoAttackManagerComponent;
// }

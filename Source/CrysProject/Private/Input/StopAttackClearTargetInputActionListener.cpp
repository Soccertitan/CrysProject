// Copyright Soccertitan 2025


#include "Input/StopAttackClearTargetInputActionListener.h"

#include "InputActionValue.h"


void UStopAttackClearTargetInputActionListener::OnInitializeListener()
{
	Super::OnInitializeListener();
	
	// GetAutoAttackManagerComponent();
}

void UStopAttackClearTargetInputActionListener::OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn)
{
	Super::OnPossessedPawnChanged(OldPawn, NewPawn);
	
	// TargetingSystemComponent = UTargetingSystemBlueprintFunctionLibrary::GetTargetingSystemComponent(NewPawn);
}

void UStopAttackClearTargetInputActionListener::OnInputActionTriggered(const FInputActionValue& Value)
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

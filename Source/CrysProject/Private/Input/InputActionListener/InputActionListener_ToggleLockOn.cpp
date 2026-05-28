// Copyright Soccertitan 2026


#include "Input/InputActionListener/InputActionListener_ToggleLockOn.h"

#include "CrimTargetingSystemBlueprintFunctionLibrary.h"
#include "CrimTargetingSystemComponent.h"

void UInputActionListener_ToggleLockOn::OnInitializeListener()
{
	Super::OnInitializeListener();
	
	TargetingSystemComponent = UCrimTargetingSystemBlueprintFunctionLibrary::GetCrimTargetingSystemComponent(GetPlayerController());
}

void UInputActionListener_ToggleLockOn::OnInputActionTriggered(const FInputActionValue& Value)
{
	Super::OnInputActionTriggered(Value);
	
	if (TargetingSystemComponent)
	{
		if (TargetingSystemComponent->IsLockedOn())
		{
			TargetingSystemComponent->SetLockOnState(false);
		}
		else
		{
			TargetingSystemComponent->SetLockOnState(true);
		}
	}
}

// Copyright Soccertitan 2026


#include "Input/InputActionListener/InputActionListener_ToggleLockOn.h"

#include "CrimTargetingSystemBlueprintFunctionLibrary.h"
#include "CrimTargetingSystemComponent.h"

void UInputActionListener_ToggleLockOn::Initialize()
{
	Super::Initialize();
	
	TargetingSystemComponent = UCrimTargetingSystemBlueprintFunctionLibrary::GetCrimTargetingSystemComponent(GetPlayerController());
}

void UInputActionListener_ToggleLockOn::InputActionTriggered(const FInputActionValue& Value)
{
	Super::InputActionTriggered(Value);
	
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

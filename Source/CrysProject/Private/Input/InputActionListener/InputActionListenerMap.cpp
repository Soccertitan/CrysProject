// Copyright Soccertitan 2026


#include "Input/InputActionListener/InputActionListenerMap.h"

#include "InputAction.h"
#include "InputActionListener.h"

void FInputActionListenerData::PostSerialize(const FArchive& Ar)
{
	EditorDisplayName = FString::Printf(TEXT("%s : %s"), *GetNameSafe(InputAction), *GetNameSafe(InputActionListener));
}

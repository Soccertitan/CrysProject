// Copyright Soccertitan 2026


#include "ActionSystem/UI/ActionSlotViewModel.h"

#include "ActionSystem/CrysAction.h"
#include "ActionSystem/UI/ActionViewModel.h"

void UActionSlotViewModel::TryActivateAction()
{
	if (ActionViewModel && ActionViewModel->GetAction())
	{
		ActionViewModel->GetAction()->TryActivateAction();
	}
}

void UActionSlotViewModel::SetActionViewModel(UActionViewModel* Value)
{
	UE_MVVM_SET_PROPERTY_VALUE(ActionViewModel, Value);
}

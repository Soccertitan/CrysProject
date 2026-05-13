// Copyright Soccertitan 2026


#include "ActionSystem/UI/ActionSlotViewModel.h"

void UActionSlotViewModel::SetActionViewModel(UActionViewModel* Value)
{
	UE_MVVM_SET_PROPERTY_VALUE(ActionViewModel, Value);
}
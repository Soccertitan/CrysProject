// Copyright Soccertitan 2026


#include "ActionSystem/UI/ActionViewModel.h"

#include "ActionSystem/CrysAction.h"
#include "ActionSystem/UI/ActionUserWidget.h"


void UActionViewModel::SetAction(UCrysAction* InAction)
{
	UE_MVVM_SET_PROPERTY_VALUE(Action, InAction);
	if (Action)
	{
		SetActionName(Action->GetActionName());
		SetIcon(Action->GetIcon());
		SetActionUserWidgetClass(Action->GetActionUserWidgetClass());
	}
}

void UActionViewModel::SetActionName(FText Value)
{
	UE_MVVM_SET_PROPERTY_VALUE(ActionName, Value);
}

void UActionViewModel::SetIcon(TSoftObjectPtr<UTexture2D> Value)
{
	UE_MVVM_SET_PROPERTY_VALUE(Icon, Value);
}

void UActionViewModel::SetActionUserWidgetClass(TSubclassOf<UActionUserWidget> Value)
{
	UE_MVVM_SET_PROPERTY_VALUE(ActionUserWidgetClass, Value);
}

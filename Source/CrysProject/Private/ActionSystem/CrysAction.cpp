// Copyright Soccertitan 2026


#include "ActionSystem/CrysAction.h"

#include "ActionSystem/UI/ActionViewModel.h"


UCrysAction::UCrysAction()
{
	ActionViewModel = UActionViewModel::StaticClass();
}

bool UCrysAction::TryActivateAction()
{
	if (CanActivateAction())
	{
		ActivateAction();
		return true;
	}
	
	return false;
}

bool UCrysAction::CanActivateAction() const
{
	if (PlayerController && K2_CanActivateAction())
	{
		return true;
	}

	return false;
}
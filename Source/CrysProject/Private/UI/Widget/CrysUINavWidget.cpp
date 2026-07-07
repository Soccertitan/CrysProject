// Copyright Soccertitan 2026


#include "UI/Widget/CrysUINavWidget.h"

void UCrysUINavWidget::GainNavigation(UUINavWidget* PreviousActiveWidget)
{
	Super::GainNavigation(PreviousActiveWidget);
	
	if (bHasNavigation && LostFocusWidget)
	{
		LostFocusWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UCrysUINavWidget::LoseNavigation(UUINavWidget* NewActiveWidget)
{
	Super::LoseNavigation(NewActiveWidget);
	
	if (!bHasNavigation && LostFocusWidget)
	{
		LostFocusWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

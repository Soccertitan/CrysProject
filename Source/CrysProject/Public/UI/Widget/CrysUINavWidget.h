// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "UINavWidget.h"
#include "CrysUINavWidget.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class CRYSPROJECT_API UCrysUINavWidget : public UUINavWidget
{
	GENERATED_BODY()
	
public:
	/** Sets the widget to Visible/Hidden automatically if the widget loses focus. */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, OptionalWidget = true))
	TObjectPtr<UWidget> LostFocusWidget;
	
	virtual void GainNavigation(UUINavWidget* PreviousActiveWidget) override;
	virtual void LoseNavigation(UUINavWidget* NewActiveWidget) override;
};

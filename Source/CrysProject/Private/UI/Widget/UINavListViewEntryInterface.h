// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UINavListViewEntryInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UUINavListViewEntryInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Used by the UINavWidgetListView to inject data into a UserWidget.
 */
class CRYSPROJECT_API IUINavListViewEntryInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/** The ListItemObject to set for the Widget. */
	UFUNCTION(BlueprintNativeEvent)
	void SetListViewItem(UObject* ListItemObject);

	/** The widget should clear its ListItemObject. */
	UFUNCTION(BlueprintNativeEvent)
	void ClearListViewItem();
};

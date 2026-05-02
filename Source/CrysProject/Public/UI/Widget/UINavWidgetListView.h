// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "UINavWidget.h"
#include "UINavWidgetListView.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUINavWidgetListViewOnFocusedComponentUpdatedSignature, UUINavComponent*, Component);

/**
 * This class takes inspiration of the ListView.h to 'virtualize' a list but to make it compatible with the
 * UINavWidget navigation system. It will manually create the EntryWidgets and place it as children. Then the ListView
 * child items will be hidden/collapsed when there is no entry.
 */
UCLASS(Abstract)
class CRYSPROJECT_API UUINavWidgetListView : public UUINavWidget
{
	GENERATED_BODY()

public:
	UUINavWidgetListView(const FObjectInitializer& ObjectInitializer);

#if WITH_EDITOR
	virtual void ValidateCompiledDefaults(class IWidgetCompilerLog& CompileLog) const override;
#endif

	/** Sets the page to the specified number. Page is clamped between 1 and MaxPages. */
	UFUNCTION(BlueprintCallable, Category = "UINavWidget|ListView")
	void GoToPage(int32 Page = 1);

	/** Tries to go to the next page. If Wrap is true, will go to the first page from the last page. */
	UFUNCTION(BlueprintCallable, Category = "UINavWidget|ListView")
	void GoToNextPage(bool bWrap = false);

	/** Tries to go to the previous page. If Wrap is true, will go to the last page from the first page. */
	UFUNCTION(BlueprintCallable, Category = "UINavWidget|ListView")
	void GoToPreviousPage(bool bWrap = false);

	/** Generates the list of items to be used in the ListView. */
	UFUNCTION(BlueprintCallable, Category = "UINavWidget|ListView", DisplayName = "SetListItems")
	void BP_SetListItems(const TArray<UObject*>& InListItems);

	template <typename ItemObjectT, typename AllocatorType = FDefaultAllocator>
	void SetListItems(const TArray<ItemObjectT, AllocatorType>& InListItems)
	{
		bIsRefreshPending = true;
		ClearListItems();
		bIsRefreshPending = false;
	
		ListItems.Reserve(InListItems.Num());
		for (const ItemObjectT ListItem : InListItems)
		{
			if (ListItem != nullptr)
			{
				ListItems.Add(ListItem);
			}
		}

		OnItemsChanged(ListItems, TArray<UObject*>());
		K2_OnItemsChanged(ListItems, TArray<UObject*>());
	}

	/** Adds a ListItem to the end of the array. */
	UFUNCTION(BlueprintCallable, Category = "UINavWidget|ListView")
	void AddListItem(UObject* Item);

	/** Removes all instances of the ListItem in the array. */
	UFUNCTION(BlueprintCallable, Category = "UINavWidget|ListView")
	void RemoveListItem(UObject* Item);

	/** Clears out the entire array of ListItems. */
	UFUNCTION(BlueprintCallable, Category = "UINavWidget|ListView")
	void ClearListItems();

	/** Gets the Item from the component. */
	UFUNCTION(BlueprintPure, Category = "UINavWidget|ListView")
	UObject* GetListItemFromComponent(UUINavComponent* Component);

	/**
	 * Gets the Item from the index.
	 * @param Index The index to get the item from. Must be between 0 and GetNumItems().
	 */
	UFUNCTION(BlueprintPure, Category = "UINavWidget|ListView")
	UObject* GetListItemFromIndex(int32 Index = 0);

	/** Returns the total number of items */
	UFUNCTION(BlueprintPure, Category = "UINavWidget|ListView")
	int32 GetNumListItems() const;

	/** Called when the ListView refreshes it's view. The value will be the currently focused Component in the ListView. */
	UPROPERTY(BlueprintAssignable, DisplayName = OnFocusedComponentUpdated, Category = "UINavWidget|ListView")
	FUINavWidgetListViewOnFocusedComponentUpdatedSignature OnFocusedComponentUpdatedDelegate;

	/** The panel widget where EntryWidgets will be added to as children. */
	UPROPERTY(BlueprintReadOnly, Category = UINavListEntry, meta=(BindWidget))
	TObjectPtr<UPanelWidget> ListView;

protected:
	/** The type of widget to create for each entry displayed in the list. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UINavListEntry,
		meta = (ExposeOnSpawn, DesignerRebuild, AllowPrivateAccess = true, MustImplement = "/Script/FinalFantasyXI.UINavListViewEntryInterface"))
	TSubclassOf<UUINavComponent> EntryWidgetClass;

	/** The number of EntryWidgets to create as children in the ListView widget. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UINavListEntry, meta = (ExposeOnSpawn, ClampMin=1, ClampMax=100))
	int32 NumberOfEntryWidgetsToCreate;

	/** Defines how to display a widget when there is no valid ListItem to set. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UINavListEntry, meta = (ExposeOnSpawn))
	ESlateVisibility VisibilityWithNoListItem;

	virtual void NativePreConstruct() override;

	/** Called when the Current page is changed to a new page. */
	UFUNCTION(BlueprintImplementableEvent, Category = UINavListEntry, DisplayName = OnCurrentPageChanged)
	void K2_OnCurrentPageChanged(int32 NewValue, int32 OldValue);
	/** Called when the Current page is changed to a new page. */
	virtual void OnCurrentPageChanged(int32 NewValue, int32 OldValue) {}

	/** Called when the Max Pages is changed. */
	UFUNCTION(BlueprintImplementableEvent, Category = UINavListEntry, DisplayName = OnMaxPagesUpdated)
	void K2_OnMaxPagesChanged(int32 NewValue, int32 OldValue);
	/** Called when the Max Pages is changed. */
	virtual void OnMaxPagesChanged(int32 NewValue, int32 OldValue) {}

	/** Called when the ListView refreshes it's view. The value will be the currently focused Component. */
	UFUNCTION(BlueprintImplementableEvent, Category = UINavListEntry, DisplayName = OnFocusedComponentUpdated)
	void K2_OnFocusedComponentUpdated(UUINavComponent* Component);
	/** Called when the ListView refreshes it's view. The value will be the currently focused Component. */
	virtual void OnFocusedComponentUpdated(UUINavComponent* Component) {}

	/** Called when the Items in the list view are added or removed. */
	UFUNCTION(BlueprintImplementableEvent, Category = "UINavListEntry", Displayname = OnItemsChanged)
	void K2_OnItemsChanged(const TArray<UObject*>& AddedItems, const TArray<UObject*>& RemovedItems);
	/** Called whenever Items are added or removed from the ListItems. */ 
	virtual void OnItemsChanged(const TArray<UObject*>& AddedItems, const TArray<UObject*>& RemovedItems);

	/** To release actors that are stored in the ListItems. */
	UFUNCTION()
	void OnListItemEndPlayed(AActor* Item, EEndPlayReason::Type EndPlayReason);
	/** To release actors that are stored in the ListItems. */
	UFUNCTION()
	void OnListItemOuterEndPlayed(AActor* ItemOuter, EEndPlayReason::Type EndPlayReason);

	/** Iterates through the ListView ChildWidgets and sets the ListEntryObject. */
	void RefreshCurrentPage();

	/** Returns true if the ListItems will be rebuilt. */
	UFUNCTION(BlueprintPure, Category = "UINavWidget|ListView")
	bool GetIsRefreshPending() const {return bIsRefreshPending;}

private:
	/** The list items. */
	UPROPERTY(Transient)
	TArray<TObjectPtr<UObject>> ListItems;

	/** The current page 'displayed' in the list. */
	UPROPERTY(BlueprintReadOnly, Transient, Category = UINavListEntry, meta = (AllowPrivateAccess=true))
	int32 CurrentPage;
	/** The max number of pages that can be navigated to in the list. */
	UPROPERTY(BlueprintReadOnly, Transient, Category = UINavListEntry, meta = (AllowPrivateAccess=true))
	int32 MaxPages;

	/** Returns true if the ListItems will be rebuilt. */
	UPROPERTY()
	bool bIsRefreshPending = false;

	/** Updates the MaxPages variable and notifies. */
	void UpdateMaxPages();
	/** Refreshes the list of items. */
	void RefreshListItems();
	/** Keeps track of references to Actors and make sure to release them when Actors are about to be removed. */
	void UpdateActorReferences(const TArray<UObject*>& AddedItems, const TArray<UObject*>& RemovedItems);

	void BroadcastCurrentPageChanged(const int32 NewValue, const int32 OldValue);
	void BroadcastMaxPagesChanged(const int32 NewValue, const int32 OldValue);
	void BroadcastFocusedComponentUpdated(UUINavComponent* Component);
};

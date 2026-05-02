// Copyright Soccertitan 2026


#include "UI/Widget/UINavWidgetListView.h"

#include "CrysLogChannels.h"
#include "UINavComponent.h"
#include "Editor/WidgetCompilerLog.h"
#include "UI/Widget/UINavListViewEntryInterface.h"

#define LOCTEXT_NAMESPACE "UMG"

UUINavWidgetListView::UUINavWidgetListView(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	NumberOfEntryWidgetsToCreate = 1;
	CurrentPage = 1;
	MaxPages = 1;
	VisibilityWithNoListItem = ESlateVisibility::Collapsed;
}

void UUINavWidgetListView::ValidateCompiledDefaults(class IWidgetCompilerLog& CompileLog) const
{
	Super::ValidateCompiledDefaults(CompileLog);

	if (!EntryWidgetClass)
	{
		CompileLog.Error(FText::Format(LOCTEXT("Error_UINavWidgetListView_MissingEntryClass", "{0} has no EntryWidgetClass specified - required for any UUINavWidgetListView to function."), FText::FromString(GetName())));
	}
	else if (!EntryWidgetClass->ImplementsInterface(UUINavListViewEntryInterface::StaticClass()))
	{
		CompileLog.Error(FText::Format(LOCTEXT("Error_UINavWidgetListView_EntryClassDoesNotImplementInterface", "'{0}' has EntryWidgetClass property set to'{1}' and that Class does not implement UUINavListViewEntryInterface - required for any UUINavWidget_ListView to function."), FText::FromString(GetName()), FText::FromString(EntryWidgetClass->GetName())));
	}
}

void UUINavWidgetListView::GoToPage(int32 Page)
{
	Page = FMath::Clamp(Page, 1, MaxPages);
	if (Page == CurrentPage)
	{
		return;
	}
	const int32 OldPage = CurrentPage;
	CurrentPage = Page;

	BroadcastCurrentPageChanged(CurrentPage, OldPage);
	RefreshCurrentPage();
}

void UUINavWidgetListView::GoToNextPage(bool bWrap)
{
	int32 NewPage = CurrentPage + 1;

	if (bWrap && NewPage > MaxPages)
	{
		GoToPage(1);
	}
	else
	{
		GoToPage(NewPage);
	}
}

void UUINavWidgetListView::GoToPreviousPage(bool bWrap)
{
	int32 NewPage = CurrentPage - 1;

	if (bWrap && NewPage < 1)
	{
		GoToPage(MaxPages);
	}
	else
	{
		GoToPage(NewPage);
	}
}

void UUINavWidgetListView::BP_SetListItems(const TArray<UObject*>& InListItems)
{
	SetListItems(InListItems);
}

void UUINavWidgetListView::AddListItem(UObject* Item)
{
	if (Item == nullptr)
	{
		UE_LOG(LogCrys, Warning, TEXT("Cannot add a null item into the ListView"));
		return;
	}

	if (ListItems.Contains(Item))
	{
		UE_LOG(LogCrys, Warning, TEXT("Cannot add a duplicate item into the ListView"));
		return;
	}

	ListItems.Add(Item);
	OnItemsChanged({Item}, {});
	K2_OnItemsChanged({Item}, {});
}

void UUINavWidgetListView::RemoveListItem(UObject* Item)
{
	if (Item == nullptr)
	{
		UE_LOG(LogCrys, Warning, TEXT("Cannot remove a null item from the ListView"));
		return;
	}

	ListItems.Remove(Item);
	OnItemsChanged({}, {Item});
	K2_OnItemsChanged({}, {Item});
}

void UUINavWidgetListView::ClearListItems()
{
	const TArray<UObject*> Removed = MoveTemp(ListItems);
	ListItems.Reset();
	OnItemsChanged({}, Removed);
	K2_OnItemsChanged({}, Removed);
}

UObject* UUINavWidgetListView::GetListItemFromComponent(UUINavComponent* Component)
{
	int32 ChildIndex = ListView->GetChildIndex(Component);
	if (ChildIndex >= 0)
	{
		int32 ItemIndex = NumberOfEntryWidgetsToCreate * (CurrentPage - 1) + ChildIndex;
		if (ListItems.IsValidIndex(ItemIndex))
		{
			return ListItems[ItemIndex];
		}
	}
	return nullptr;
}

UObject* UUINavWidgetListView::GetListItemFromIndex(int32 Index)
{
	if (ListItems.IsValidIndex(Index))
	{
		return ListItems[Index];
	}
	return nullptr;
}

int32 UUINavWidgetListView::GetNumListItems() const
{
	return ListItems.Num();
}

void UUINavWidgetListView::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (EntryWidgetClass)
	{
		if (ListView)
		{
			ListView->ClearChildren();
		
			for (int32 idx = 0; idx < NumberOfEntryWidgetsToCreate; idx++)
			{
				UUINavComponent* NewNavComp = CreateWidget<UUINavComponent>(this, EntryWidgetClass);
				if (IsDesignTime())
				{
					NewNavComp->SetVisibility(ESlateVisibility::Visible);
				}
				else
				{
					NewNavComp->SetVisibility(VisibilityWithNoListItem);
				}
				IUINavListViewEntryInterface::Execute_ClearListViewItem(NewNavComp);
				ListView->AddChild(NewNavComp);
			}
		}
	}
}

void UUINavWidgetListView::OnItemsChanged(const TArray<UObject*>& AddedItems, const TArray<UObject*>& RemovedItems)
{
	if (!GetIsRefreshPending())
	{
		RefreshListItems();
	}
	UpdateActorReferences(AddedItems, RemovedItems);
}

void UUINavWidgetListView::OnListItemEndPlayed(AActor* Item, EEndPlayReason::Type EndPlayReason)
{
	RemoveListItem(Item);
}

void UUINavWidgetListView::OnListItemOuterEndPlayed(AActor* ItemOuter, EEndPlayReason::Type EndPlayReason)
{
	for (int32 ItemIndex = ListItems.Num() - 1; ItemIndex >= 0; --ItemIndex)
	{
		UObject* Item = ListItems[ItemIndex];
		if (Item->IsIn(ItemOuter))
		{
			RemoveListItem(Item);
		}
	}
}

void UUINavWidgetListView::RefreshCurrentPage()
{
	const int32 StartingListItemIndex = FMath::Max(NumberOfEntryWidgetsToCreate * (CurrentPage - 1), 0);

	for (int32 idx = 0; idx < NumberOfEntryWidgetsToCreate; idx++)
	{
		const int32 ListItemIndex = StartingListItemIndex + idx;
		if (UUINavComponent* Widget = Cast<UUINavComponent>(ListView->GetChildAt(idx)))
		{
			if (ListItems.IsValidIndex(ListItemIndex))
			{
				IUINavListViewEntryInterface::Execute_SetListViewItem(Widget, ListItems[ListItemIndex]);
				Widget->SetVisibility(ESlateVisibility::Visible);
			}
			else
			{
				if (Widget->HasFocusedDescendants() &&
					VisibilityWithNoListItem != ESlateVisibility::Visible)
				{
					int32 ChildIndex = idx - 1;
					bool bSetFocusOnChild = false;
					do
					{
						UWidget* PreviousWidget = ListView->GetChildAt(ChildIndex);
						if (PreviousWidget && PreviousWidget->GetVisibility() == ESlateVisibility::Visible)
						{
							PreviousWidget->SetFocus();
							bSetFocusOnChild = true;
						}
						ChildIndex--;
					}
					while (ChildIndex >= 0 && !bSetFocusOnChild);
	
					if (!bSetFocusOnChild)
					{
						if (UUINavComponent* InitialFocusComp = GetInitialFocusComponent())
						{
							InitialFocusComp->SetFocus();	
						}
					}
				}
				Widget->SetVisibility(VisibilityWithNoListItem);
				IUINavListViewEntryInterface::Execute_ClearListViewItem(Widget);
			}
			
			if (Widget->HasFocusedDescendants())
			{
				BroadcastFocusedComponentUpdated(Widget);
			}
		}
	}
}

void UUINavWidgetListView::UpdateMaxPages()
{
	int32 OldMaxPages = MaxPages;
	if (ListItems.Num() > 0)
	{
		MaxPages = FMath::Max(FMath::DivideAndRoundUp(ListItems.Num(), NumberOfEntryWidgetsToCreate), 1);
	}
	else
	{
		MaxPages = 1;
	}

	if (OldMaxPages != MaxPages)
	{
		BroadcastMaxPagesChanged(MaxPages, OldMaxPages);
	}
}

void UUINavWidgetListView::RefreshListItems()
{
	UpdateMaxPages();
	if (CurrentPage > MaxPages)
	{
		GoToPage(MaxPages);
	}
	else
	{
		RefreshCurrentPage();
	}
}

void UUINavWidgetListView::UpdateActorReferences(const TArray<UObject*>& AddedItems, const TArray<UObject*>& RemovedItems)
{
	for (UObject* AddedItem : AddedItems)
	{
		if (AActor* AddedActor = Cast<AActor>(AddedItem))
		{
			AddedActor->OnEndPlay.AddDynamic(this, &UUINavWidgetListView::OnListItemEndPlayed);
		}
		else if (AActor* AddedItemOuterActor = AddedItem->GetTypedOuter<AActor>())
		{
			// Unique so that we don't spam events for shared actor outers but this also means we can't
			// unsubscribe when processing RemovedItems
			AddedItemOuterActor->OnEndPlay.AddUniqueDynamic(this, &UUINavWidgetListView::OnListItemOuterEndPlayed);
		}
	}
	for (UObject* RemovedItem : RemovedItems)
	{
		if (AActor* RemovedActor = Cast<AActor>(RemovedItem))
		{
			RemovedActor->OnEndPlay.RemoveDynamic(this, &UUINavWidgetListView::OnListItemEndPlayed);
		}
	}
}

void UUINavWidgetListView::BroadcastCurrentPageChanged(const int32 NewValue, const int32 OldValue)
{
	OnCurrentPageChanged(NewValue, OldValue);
	K2_OnCurrentPageChanged(NewValue, OldValue);
}

void UUINavWidgetListView::BroadcastMaxPagesChanged(const int32 NewValue, const int32 OldValue)
{
	OnMaxPagesChanged(NewValue, OldValue);
	K2_OnMaxPagesChanged(NewValue, OldValue);
}

void UUINavWidgetListView::BroadcastFocusedComponentUpdated(UUINavComponent* Component)
{
	OnFocusedComponentUpdated(Component);
	K2_OnFocusedComponentUpdated(Component);
	OnFocusedComponentUpdatedDelegate.Broadcast(Component);
}

// Copyright Soccertitan 2026


#include "UI/Widget/CrysWidgetBlueprintFunctionLibrary.h"

#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Data/SelectorPosition.h"


TArray<UWidget*> UCrysWidgetBlueprintFunctionLibrary::TraverseWidgetHierarchy(UUserWidget* UserWidget, TSubclassOf<UUserWidget> TestClass)
{
	TArray<UWidget*> OutWidgets;

	if (IsValid(UserWidget) && IsValid(TestClass))
	{
		TArray<UWidget*> Widgets;
		UserWidget->WidgetTree->GetAllWidgets(Widgets);
		for (UWidget* Widget : Widgets)
		{
			if (Widget->IsA(TestClass))
			{
				OutWidgets.Add(Widget);
			}
		}
	}
	
	return OutWidgets;
}

FVector2D UCrysWidgetBlueprintFunctionLibrary::GetLocation(UWidget* Widget, const ESelectorPosition Offset, const bool bUseViewportPosition)
{
	if (!Widget)
	{
		return FVector2D();
	}
	
	const FGeometry Geom = Widget->GetCachedGeometry();
	const FVector2D LocalSize = Geom.GetLocalSize();
	FVector2D LocalPosition = FVector2D::ZeroVector;
	switch (Offset)
	{
	case ESelectorPosition::Position_Center:
		LocalPosition = LocalSize / 2;
		break;
	case ESelectorPosition::Position_Top:
		LocalPosition = FVector2D(LocalSize.X / 2, 0.f);
		break;
	case ESelectorPosition::Position_Bottom:
		LocalPosition = FVector2D(LocalSize.X / 2, LocalSize.Y);
		break;
	case ESelectorPosition::Position_Left:
		LocalPosition = FVector2D(0.f, LocalSize.Y / 2);
		break;
	case ESelectorPosition::Position_Right:
		LocalPosition = FVector2D(LocalSize.X, LocalSize.Y / 2);
		break;
	case ESelectorPosition::Position_Top_Right:
		LocalPosition = FVector2D(LocalSize.X, 0.f);
		break;
	case ESelectorPosition::Position_Top_Left:
		LocalPosition = FVector2D(0.f, 0.f);
		break;
	case ESelectorPosition::Position_Bottom_Right:
		LocalPosition = FVector2D(LocalSize.X, LocalSize.Y);
		break;
	case ESelectorPosition::Position_Bottom_Left:
		LocalPosition = FVector2D(0.f, LocalSize.Y);
		break;
	}

	FVector2D PixelPos, ViewportPos;
	USlateBlueprintLibrary::LocalToViewport(Widget->GetWorld(), Geom, LocalPosition, PixelPos, ViewportPos);
	return bUseViewportPosition ? ViewportPos : PixelPos;
}

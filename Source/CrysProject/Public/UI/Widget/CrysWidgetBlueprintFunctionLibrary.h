// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CrysWidgetBlueprintFunctionLibrary.generated.h"

enum class ESelectorPosition : uint8;
class UWidget;

/**
 * 
 */
UCLASS()
class CRYSPROJECT_API UCrysWidgetBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	/**
	 * @param UserWidget The widget we want to traverse through.
	 * @param TestClass The widget class we are searching for.
	 * @return All user widgets that are of the specified UserWidget Class.
	 */
	UFUNCTION(BlueprintCallable, Category = "WidgetFunctionLibrary", meta=(DeterminesOutputType=TestClass, DefaultToSelf = UserWidget))
	static TArray<UWidget*> TraverseWidgetHierarchy(UUserWidget* UserWidget, TSubclassOf<UUserWidget> TestClass);
	
	template<class UserWidgetType, class AllocatorType>
	static void TraverseWidgetHierarchy(UUserWidget* UserWidget, TArray<UserWidgetType, AllocatorType>& OutUserWidgets)
	{
		typedef TPointedToType<UserWidgetType> T;

		OutUserWidgets.Reset();
		if (IsValid(UserWidget))
		{
			TArray<UWidget*> Widgets;
			UserWidget->WidgetTree->GetAllWidgets(Widgets);
			for (UWidget* Widget : Widgets)
			{
				if (Widget->IsA(T::StaticClass()))
				{
					OutUserWidgets.Add((T*)Widget);
				}
			}
		}
	}
	
	UFUNCTION(BlueprintPure, Category = "WidgetFunctionLibrary")
	static FVector2D GetLocation(UWidget* Widget, const ESelectorPosition Offset, const bool bUseViewportPosition);
};

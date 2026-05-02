// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
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
	
	UFUNCTION(BlueprintPure, Category = "WidgetFunctionLibrary")
	static FVector2D GetLocation(UWidget* Widget, const ESelectorPosition Offset, const bool bUseViewportPosition);
};

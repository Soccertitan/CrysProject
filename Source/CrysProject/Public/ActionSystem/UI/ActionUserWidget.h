// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/CrysUserWidget.h"
#include "ActionUserWidget.generated.h"

class UActionViewModel;

/**
 * Contains a functions for sending an ActionViewModel.
 */
UCLASS(Abstract)
class CRYSPROJECT_API UActionUserWidget : public UCrysUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetViewModel(UActionViewModel* ViewModel);
};

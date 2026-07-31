// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/CrysUINavWidget.h"
#include "AbilityContainerWidget.generated.h"

class UAbilitySlotWidget;
class UAbilityInputManagerViewModel;

/**
 * Automates setting up Ability Slot Widgets.
 */
UCLASS()
class CRYSPROJECT_API UAbilityContainerWidget : public UCrysUINavWidget
{
	GENERATED_BODY()
	
public:
	
	// Set's the ViewModel on all AbilitySlotWidgets.
	UFUNCTION(BlueprintCallable)
	void SetAbilitySlotWidgetsViewModel(UAbilityInputManagerViewModel* AbilityInputManagerViewModel);
	
#if WITH_EDITOR
	virtual void ValidateCompiledDefaults(class IWidgetCompilerLog& CompileLog) const override;
#endif
	
protected:
	UPROPERTY(BlueprintReadOnly)
	TArray<TObjectPtr<UAbilitySlotWidget>> AbilitySlotWidgets;
	
	virtual void NativePreConstruct() override;
	
private:
	// The AbilitySlotWidget to create in the slot.
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAbilitySlotWidget> AbilitySlotWidgetClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn, ClampMin = 0, AllowPrivateAccess))
	int32 InputSet = 0;
};

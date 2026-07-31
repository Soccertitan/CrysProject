// Copyright Soccertitan 2026


#include "UI/Widget/Ability/AbilityContainerWidget.h"

#include "Editor/WidgetCompilerLog.h"
#include "Input/UI/AbilityInputManagerViewModel.h"
#include "UI/Widget/CrysWidgetBlueprintFunctionLibrary.h"
#include "UI/Widget/Ability/AbilitySlotOverlay.h"
#include "UI/Widget/Ability/AbilitySlotWidget.h"

#define LOCTEXT_NAMESPACE "UMG"

void UAbilityContainerWidget::SetAbilitySlotWidgetsViewModel(UAbilityInputManagerViewModel* AbilityInputManagerViewModel)
{
	if (!AbilityInputManagerViewModel)
	{
		return;
	}
	
	for (UAbilitySlotWidget* AbilitySlotWidget : AbilitySlotWidgets)
	{
		UAbilityInputSlotViewModel* ViewModel = AbilityInputManagerViewModel->FindOrCreateInputSlotViewModel(AbilitySlotWidget->InputSlot, InputSet);
		AbilitySlotWidget->K2_SetAbilityInputSlotViewModel(ViewModel);
	}
}

void UAbilityContainerWidget::ValidateCompiledDefaults(class IWidgetCompilerLog& CompileLog) const
{
	Super::ValidateCompiledDefaults(CompileLog);
	
	if (!AbilitySlotWidgetClass)
	{
		CompileLog.Error(FText::Format(LOCTEXT("Error_UAbilityContainerWidget_MissingAbilitySlotWidgetClass", "{0} has no AbilitySlotWidgetClass specified - required for any UAbilityContainerWidget to function."), FText::FromString(GetName())));
	}
}

void UAbilityContainerWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	if (AbilitySlotWidgetClass)
	{
		TArray<UAbilitySlotOverlay*> AbilitySlots;
		UCrysWidgetBlueprintFunctionLibrary::TraverseWidgetHierarchy(this, AbilitySlots);
		
		for (UAbilitySlotOverlay* AbilitySlot : AbilitySlots)
		{
			UAbilitySlotWidget* AbilitySlotWidget = CreateWidget<UAbilitySlotWidget>(this, AbilitySlotWidgetClass);
			AbilitySlot->SetAbilitySlotWidget(AbilitySlotWidget);
		}
	}
	UCrysWidgetBlueprintFunctionLibrary::TraverseWidgetHierarchy(this, AbilitySlotWidgets);
}

#undef LOCTEXT_NAMESPACE

// Copyright Soccertitan 2026


#include "UI/ViewModel/CrysViewModelContextResolver.h"

#include "CrysBlueprintFunctionLibrary.h"
#include "Blueprint/UserWidget.h"
#include "UI/ViewModel/CrysViewModel.h"

UObject* UCrysViewModelContextResolver::CreateInstance(const UClass* ExpectedType, const UUserWidget* UserWidget, const UMVVMView* View) const
{
	return FindOrCreateViewModel(ExpectedType, UserWidget);
}

bool UCrysViewModelContextResolver::DoesSupportViewModelClass(const UClass* Class) const
{
	if (Super::DoesSupportViewModelClass(Class))
	{
		if (!Class->IsChildOf(UCrysViewModel::StaticClass()))
		{
			return false;
		}
	}

	return true;
}

UCrysViewModel* UCrysViewModelContextResolver::FindOrCreateViewModel(const UClass* ExpectedType, const UUserWidget* UserWidget) const
{
	TSubclassOf<UCrysViewModel> Class(const_cast<UClass*>(ExpectedType));
	return UCrysBlueprintFunctionLibrary::FindOrCreateViewModel(Class, UserWidget->GetOwningPlayer());
}

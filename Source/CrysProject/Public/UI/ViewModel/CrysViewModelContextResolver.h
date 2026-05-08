// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "View/MVVMViewModelContextResolver.h"
#include "CrysViewModelContextResolver.generated.h"

class UCrysViewModel;

/**
 * Retrieves a ViewModel from the CrysHUD.
 */
UCLASS()
class CRYSPROJECT_API UCrysViewModelContextResolver : public UMVVMViewModelContextResolver
{
	GENERATED_BODY()
	
public:
	virtual UObject* CreateInstance(const UClass* ExpectedType, const UUserWidget* UserWidget, const UMVVMView* View) const override;
	virtual bool DoesSupportViewModelClass(const UClass* Class) const override;

protected:
	UCrysViewModel* FindOrCreateViewModel(const UClass* ExpectedType, const UUserWidget* UserWidget) const;
};

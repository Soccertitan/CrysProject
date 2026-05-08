// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CrysHUD.generated.h"

class UCrysViewModel;
/**
 * The base HUD class used in this project.
 */
UCLASS()
class CRYSPROJECT_API ACrysHUD : public AHUD
{
	GENERATED_BODY()

public:
	/**
	 * If a ViewModel of class already exists return that ViewModel. Otherwise, creates a new ViewModel of
	 * the class type. Then initializes the ViewModel with the owning HUD's PlayerController.
	 */
	template<class T>
	T* FindOrCreateViewModel()
	{
		static_assert(TPointerIsConvertibleFromTo<T, const UCrysViewModel>::Value,
			"'T' template parameter to FindOrCreateViewModel must be derived from CrysViewModel");
		return (T*)FindOrCreateViewModel(T::StaticClass());
	}
	
	UCrysViewModel* FindOrCreateViewModel(const TSubclassOf<UCrysViewModel> ViewModelClass);

	/** Creates the HUDWidget from the class and adds it to the viewport. */
	virtual void CreateHUDWidget();
	
	UFUNCTION(BlueprintPure, Category = "HUD")
	UUserWidget* GetHUDWidget() const { return HUDWidget; }
	
private:
	UPROPERTY()
	TObjectPtr<UUserWidget> HUDWidget;

	// The widget to spawn when InitializeHUD is called.
	UPROPERTY(EditDefaultsOnly, Category = "Crys")
	TSubclassOf<UUserWidget> HUDWidgetClass;

	/** Stores a collection of CrysViewModels. Each view model in the array is unique. */
	UPROPERTY()
	TArray<TWeakObjectPtr<UCrysViewModel>> ViewModels;
};

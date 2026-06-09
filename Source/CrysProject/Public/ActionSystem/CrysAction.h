// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CrysAction.generated.h"

class UActionUserWidget;
class UActionViewModel;
class ACrysPlayerController;
class UCrysActionManagerComponent;

/**
 * Defines an action that can be mapped to an ActionManager or activated on its own.
 */
UCLASS(BlueprintType, Blueprintable, Abstract)
class CRYSPROJECT_API UCrysAction : public UObject
{
	GENERATED_BODY()
	
public:
	UCrysAction();
	
	UFUNCTION(BlueprintPure, Category = "CrysAction")
	FText GetActionName() const {return ActionName;}
	
	UFUNCTION(BlueprintPure, Category = "CrysAction")
	TSoftObjectPtr<UTexture2D> GetIcon() const {return Icon;}
	
	TSubclassOf<UActionUserWidget> GetActionUserWidgetClass() const { return ActionUserWidgetClass; }
	TSubclassOf<UActionViewModel> GetActionViewModel() const { return ActionViewModel; }
	
	UFUNCTION(BlueprintCallable, Category = "CrysAction")
	bool TryActivateAction();

	UFUNCTION(BlueprintPure, Category = "CrysAction")
	virtual bool CanActivateAction() const;

protected:
	virtual void Initialize() {}

	UFUNCTION(BlueprintNativeEvent, DisplayName = "ActivateAction")
	void ActivateAction();
	virtual void ActivateAction_Implementation() {}
	
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "CanActivateAction")
	bool K2_CanActivateAction() const;
	
	UFUNCTION(BlueprintPure, Category = "CrysAction")
	ACrysPlayerController* GetPlayerController() const { return PlayerController; }
	
private:
	/** The name of the action. */
	UPROPERTY(EditDefaultsOnly)
	FText ActionName;
	
	/** The user facing icon on the hotbar. */
	UPROPERTY(EditDefaultsOnly, meta = (AssetBundles = "UI"))
	TSoftObjectPtr<UTexture2D> Icon;
	
	/** The widget to spawn to display information on an ActionBar. */
	UPROPERTY(EditDefaultsOnly, meta = (AssetBundles = "UI"))
	TSubclassOf<UActionUserWidget> ActionUserWidgetClass;
	
	/** Displays information about the action in a widget. */
	UPROPERTY(EditDefaultsOnly, NoClear)
	TSubclassOf<UActionViewModel> ActionViewModel;
	
	UPROPERTY()
	TObjectPtr<ACrysPlayerController> PlayerController;
	
	friend UCrysActionManagerComponent;
};

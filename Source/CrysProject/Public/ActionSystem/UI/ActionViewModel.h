// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "ActionViewModel.generated.h"

class UActionUserWidget;
class UActionManagerViewModel;
class UCrysAction;

/**
 * 
 */
UCLASS()
class CRYSPROJECT_API UActionViewModel : public UMVVMViewModelBase
{
	GENERATED_BODY()
	
public:
	FText GetActionName() const { return ActionName; }
	TSoftObjectPtr<UTexture2D> GetIcon() const { return Icon; }
	TSubclassOf<UActionUserWidget> GetWidgetClass() const { return WidgetClass; }
	UCrysAction* GetAction() const { return Action; }

protected:
	virtual void SetAction(UCrysAction* InAction);
	void SetActionName(FText Value);
	void SetIcon(TSoftObjectPtr<UTexture2D> Value);

private:
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Category = "Viewmodel|ActionBar", meta = (AllowPrivateAccess = true))
	FText ActionName;
	
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Category = "Viewmodel|ActionBar", meta = (AllowPrivateAccess = true))
	TSoftObjectPtr<UTexture2D> Icon;
	
	/** The widget to spawn to display information on the ActionBar. */
	UPROPERTY(EditDefaultsOnly, Getter, BlueprintReadOnly, Category = "Viewmodel|ActionBar", meta = (AllowPrivateAccess = true))
	TSubclassOf<UActionUserWidget> WidgetClass;
	
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Category = "Viewmodel|ActionBar", meta = (AllowPrivateAccess = true))
	TObjectPtr<UCrysAction> Action;
	
	friend UActionManagerViewModel;
};

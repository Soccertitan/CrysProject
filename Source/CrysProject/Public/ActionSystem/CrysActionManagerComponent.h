// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "CrysActionTypes.h"
#include "Components/ActorComponent.h"
#include "CrysActionManagerComponent.generated.h"

class UInputMappingContext;
class ACrysPlayerController;
class UCrysAction;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCrysActionManagerInputModeSignature, ECrysActionInputMode, InputMode);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCrysActionManagerActionSetSignature, int32, ActionSetIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FCrysActionManagerActionMapUpdateSignature, UCrysAction*, Action, const FGameplayTag&, InputTag, const int32, ActionIndex, int32, ActionSetIndex);

/**
 * A class that manages the actions that are activatable by InputTag. This component is designed to only live on the CrysPlayerController.
 * This is like a Hotbar, ActionBar, QuickSlot functionality.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CRYSPROJECT_API UCrysActionManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCrysActionManagerComponent();
	virtual void BeginPlay() override;
	
	/** Called when input mode changes. */
	UPROPERTY(BlueprintAssignable, DisplayName = "OnInputModeChanged")
	FCrysActionManagerInputModeSignature OnInputModeChangedDelegate;
	
	/** Called whenever an action map is changed/added/removed. */
	UPROPERTY(BlueprintAssignable, DisplayName = "OnActionMapUpdated")
	FCrysActionManagerActionMapUpdateSignature OnActionMapUpdatedDelegate;
	
	/** Called when the chosen action set has changed. */
	UPROPERTY(BlueprintAssignable, DisplayName = "OnActiveActionSetChanged")
	FCrysActionManagerActionSetSignature OnActiveActionSetChangedDelegate;
	
	/** Finds an action given an InputTag or ActionIndex. Set InputTag to none to use the action index. */
	UFUNCTION(BlueprintPure, Category = "CrysActionManager")
	UCrysAction* FindAction(UPARAM(meta = (Categories="Input")) const FGameplayTag InputTag, const int32 ActionIndex = -1, const int32 ActionSetIndex = 0) const;
	
	UFUNCTION(BlueprintPure, Category = "CrysActionManager")
	UCrysAction* FindActionByClass(const TSubclassOf<UCrysAction> ActionClass) const;

	UFUNCTION(BlueprintPure, Category = "CrysActionManager")
	const TArray<FCrysActionMapInstance>& GetActionMapInstances() const;
	
	/** Tries to activate the action for the given InputTag/ActionIndex. */
	UFUNCTION(BlueprintCallable, Category = "CrysActionManager")
	bool TryActivateActiveAction(UPARAM(meta = (Categories = "Input")) const FGameplayTag InputTag, const int32 ActionIndex);
	
	/** Creates an action and tries to activate it once. */
	UFUNCTION(BlueprintCallable, Category = "CrysActionManager")
	bool FindOrCreateActionAndTryActivateOnce(const TSubclassOf<UCrysAction> ActionClass);
	
	/** Sets and creates an action for the specified InputTag and Index. If ActionClass is invalid, will clear out instead. */
	UFUNCTION(BlueprintCallable, Category = "CrysActionManager")
	void SetAction(UPARAM(meta = (Categories = "Input")) const FGameplayTag InputTag, int32 ActionIndex, const int32 ActionSetIndex, const TSubclassOf<UCrysAction> ActionClass);
	
	/** Clears the action mappings and updates it with the passed in values. */
	UFUNCTION(BlueprintCallable, Category = "CrysActionManager")
	void SetActionMap(const FCrysActionMap& InActionMap, const int32 ActionSetIndex);
	
	/** Removes an action from the map for a given an Index (ActionSet) and InputTag. */
	UFUNCTION(BlueprintCallable, Category = "CrysActionManager")
	void ClearAction(UPARAM(meta = (Categories = "Input")) const FGameplayTag InputTag, const int32 ActionIndex, const int32 ActionSetIndex);
	
	/** Removes all action mappings for the ActionSet. */
	UFUNCTION(BlueprintCallable, Category = "CrysActionManager")
	void ClearActions(const int32 ActionSetIndex);

	/** Switches the active ActionSet to the specified ActionSetIndex. */
	UFUNCTION(BlueprintCallable, Category = "CrysActionManager")
	void SetActiveActionSet(const int32 Index);

	/** 
	 * A helper function to go to the next valid ActionSet. Looping between Index 1 and the max number of Indexes. 
	 * Use SetActionSetIndex to force a switch to Index 0.
	 */
	UFUNCTION(BlueprintCallable, Category = "CrysActionManager")
	void SwitchToNextActionSet(const bool bIncrementIndex = true);
	
	UFUNCTION(BlueprintPure, Category = "CrysActionManager")
	int32 GetActiveActionSetIndex() const { return ActiveActionSetIndex; }

	UFUNCTION(BlueprintPure, Category = "CrysActionManager")
	bool IsActionSetEmpty(const int32 Index) const;
	
	UFUNCTION(BlueprintPure, Category = "CrysActionManager")
	ECrysActionInputMode GetInputMode() const { return InputMode; }
	
	UFUNCTION(BlueprintCallable, Category = "CrysActionManager")
	void SetInputMode(const ECrysActionInputMode InputMode);
	
protected:
	virtual void OnRegister() override;
	
private:
	/** 
	 * The active mappings. 
	 * The Array Index is an "ActionSet" and the TMap is the InputTag to Action Map.
	 */
	UPROPERTY()
	TArray<FCrysActionMapInstance> ActionMapInstances;

	/** A cache of actions that were created that can be activated many times. */
	UPROPERTY()
	TArray<TObjectPtr<UCrysAction>> ActionPool;

	UPROPERTY(EditAnywhere, Category = "Input")
	ECrysActionInputMode InputMode = ECrysActionInputMode::Gamepad;
	UPROPERTY(EditAnywhere, Category = "Input")
	TMap<ECrysActionInputMode, TObjectPtr<UInputMappingContext>> InputModeMappingContextMap;
	UPROPERTY(EditAnywhere, Category = "Input")
	int32 InputMappingContextPriority = 0;
	
	UPROPERTY(EditAnywhere, meta = (ClampMin = 1))
	int32 MaxCacheSize = 10;

	/** As items are added to the ActionPool cache. Increment by 1, then reset once we reach max cache size. */
	int32 CacheIndex = 0;

	int32 ActiveActionSetIndex = 1;

	UPROPERTY()
	TObjectPtr<ACrysPlayerController> PlayerController;

	UCrysAction* CreateActionInternal(const TSubclassOf<UCrysAction>& ActionClass);
	
	void SetInputModeInternal(const ECrysActionInputMode InputMode);
};

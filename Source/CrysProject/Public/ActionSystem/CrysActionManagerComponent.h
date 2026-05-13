// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "CrysActionTypes.h"
#include "Components/ActorComponent.h"
#include "CrysActionManagerComponent.generated.h"

class ACrysPlayerController;
class UCrysAction;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCrysActionManagerActionSetSignature, int32, Index);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FCrysActionManagerActionUpdateSignature, UCrysAction*, Action,
                                               const FGameplayTag&, InputTag, int32, Index);

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
	
	/** Called whenever an action map is changed/added/removed. */
	UPROPERTY(BlueprintAssignable, DisplayName = "OnActionMapUpdated")
	FCrysActionManagerActionUpdateSignature OnActionMapUpdatedDelegate;
	
	/** Called when the chosen action set has changed. */
	UPROPERTY(BlueprintAssignable, DisplayName = "OnActionSetSelected")
	FCrysActionManagerActionSetSignature OnActionSetSelectedDelegate;
	
	UFUNCTION(BlueprintPure, Category = "CrysActionManager")
	UCrysAction* FindAction(UPARAM(meta = (Categories="Input")) const FGameplayTag InputTag, const int32 Index) const;
	
	UFUNCTION(BlueprintPure, Category = "CrysActionManager")
	UCrysAction* FindActionByClass(const TSubclassOf<UCrysAction> ActionClass) const;

	UFUNCTION(BlueprintPure, Category = "CrysActionManager")
	const TArray<FCrysActionMapInstance>& GetActionMapInstances() const;
	
	/** Creates and inits an action of the ActionClass that must be kept alive by the caller. */
	UFUNCTION(BlueprintCallable, Category = "CrysActionManager")
	UCrysAction* CreateAction(const TSubclassOf<UCrysAction> ActionClass);
	
	/** Creates an action and calls activate once on it. */
	UFUNCTION(BlueprintCallable, Category = "CrysActionManager")
	bool CreateActionAndTryActivateOnce(const TSubclassOf<UCrysAction> ActionClass);
	
	/** Tries to activate an action at the current ActionSetIndex for a given InputTag. */
	UFUNCTION(BlueprintCallable, Category = "CrysActionManager")
	bool TryActivateAction(UPARAM(meta = (Categories="Input")) const FGameplayTag InputTag);
	
	/** Tries to activate an action at the specified Index (ActionSet) for a given InputTag. */
	UFUNCTION(BlueprintCallable, Category = "CrysActionManager")
	bool TryActivateActionAtIndex(UPARAM(meta = (Categories="Input")) const FGameplayTag InputTag, const int32 Index);
	
	/** Sets and creates an action for the specified InputTag and Index. If ActionClass is invalid, will clear out instead. */
	UFUNCTION(BlueprintCallable, Category = "CrysActionManager")
	void SetAction(UPARAM(meta = (Categories = "Input")) const FGameplayTag InputTag, const int32 Index, const TSubclassOf<UCrysAction> ActionClass);
	
	/** Clears the action mappings and updates it with the passed in values. */
	UFUNCTION(BlueprintCallable, Category = "CrysActionManager")
	void SetActionMap(const FCrysActionMap& InActionMap, const int32 Index);
	
	/** Removes an action from the map for a given an Index (ActionSet) and InputTag. */
	UFUNCTION(BlueprintCallable, Category = "CrysActionManager")
	void ClearAction(UPARAM(meta = (Categories = "Input")) const FGameplayTag InputTag, const int32 Index);
	
	/** Removes all action mappings at Index. */
	UFUNCTION(BlueprintCallable, Category = "CrysActionManager")
	void ClearActionMap(const int32 Index);

	/** Switches the active ActionSet to the specified ActionSetIndex. */
	UFUNCTION(BlueprintCallable, Category = "CrysActionManager")
	void SetActionSetIndex(const int32 Index);

	/** 
	 * A helper function to go to the next valid ActionSet. Looping between Index 1 and the max number of Indexes. 
	 * Use SetActionSetIndex to force a switch to Index 0.
	 */
	UFUNCTION(BlueprintCallable, Category = "CrysActionManager")
	void SwitchToNextActionSet(const bool bIncrementIndex = true);
	
	UFUNCTION(BlueprintPure, Category = "CrysActionManager")
	int32 GetActionSetIndex() const { return ActionSetIndex; }

	UFUNCTION(BlueprintPure, Category = "CrysActionManager")
	bool IsActionSetEmpty(const int32 Index) const;
	
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

	UPROPERTY(EditAnywhere, meta = (ClampMin = 1))
	int32 MaxCacheSize = 10;

	/** As items are added to the ActionPool cache. Increment by 1, then reset once we reach max cache size. */
	int32 CacheIndex = 0;

	int32 ActionSetIndex = 1;

	UPROPERTY()
	TObjectPtr<ACrysPlayerController> PlayerController;

	UCrysAction* InternalCreateAction(const TSubclassOf<UCrysAction>& ActionClass);
};

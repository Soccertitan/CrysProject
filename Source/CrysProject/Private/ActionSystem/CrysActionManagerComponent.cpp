// Copyright Soccertitan 2026


#include "ActionSystem/CrysActionManagerComponent.h"

#include "CrysLogChannels.h"
#include "EnhancedInputSubsystems.h"
#include "ActionSystem/CrysAction.h"
#include "Player/CrysPlayerController.h"


UCrysActionManagerComponent::UCrysActionManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	ActionPool.SetNum(MaxCacheSize);
}

void UCrysActionManagerComponent::BeginPlay()
{
	Super::BeginPlay();
	
	SetInputModeInternal(InputMode);
}

void UCrysActionManagerComponent::OnRegister()
{
	Super::OnRegister();
	PlayerController = Cast<ACrysPlayerController>(GetOwner());
	check(PlayerController);
}

UCrysAction* UCrysActionManagerComponent::FindAction(const FGameplayTag InputTag, const int32 ActionIndex, const int32 ActionSetIndex) const
{
	if (ActionMapInstances.IsValidIndex(ActionSetIndex))
	{
		if (InputTag.IsValid())
		{
			if (ActionMapInstances[ActionSetIndex].InputTagActionMap.Contains(InputTag))
			{
				return *ActionMapInstances[ActionSetIndex].InputTagActionMap.Find(InputTag);
			}
		}
		else if (ActionMapInstances[ActionSetIndex].Actions.IsValidIndex(ActionIndex))
		{
			return ActionMapInstances[ActionSetIndex].Actions[ActionIndex];
		}
	}
	return nullptr;
}

UCrysAction* UCrysActionManagerComponent::FindActionByClass(const TSubclassOf<UCrysAction> ActionClass) const
{
	if (ActionClass)
	{
		for (const TObjectPtr<UCrysAction>& Action : ActionPool)
		{
			if (Action && Action->GetClass() == ActionClass)
			{
				return Action;
			}
		}

		for (const FCrysActionMapInstance& Map : ActionMapInstances)
		{
			for (const TTuple<FGameplayTag, TObjectPtr<UCrysAction>>& Pair : Map.InputTagActionMap)
			{
				if (Pair.Value->GetClass() == ActionClass)
				{
					return Pair.Value;
				}
			}
			for (const TObjectPtr<UCrysAction>& Action : Map.Actions)
			{
				if (Action && Action->GetClass() == ActionClass)
				{
					return Action;
				}
			}
		}
	}
	return nullptr;
}

const TArray<FCrysActionMapInstance>& UCrysActionManagerComponent::GetActionMapInstances() const
{
	return ActionMapInstances;
}

bool UCrysActionManagerComponent::TryActivateActiveAction(const FGameplayTag InputTag, const int32 ActionIndex)
{
	if (ActionMapInstances.IsValidIndex(ActiveActionSetIndex))
	{
		FCrysActionMapInstance& ActionMapInstance = ActionMapInstances[ActiveActionSetIndex];
		if (InputTag.IsValid())
		{
			if (ActionMapInstance.InputTagActionMap.Contains(InputTag))
			{
				UCrysAction* Action = *ActionMapInstance.InputTagActionMap.Find(InputTag);
				return Action->TryActivateAction();
			}
		}
		else if (ActionMapInstance.Actions.IsValidIndex(ActionIndex))
		{
			if (ActionMapInstance.Actions[ActionIndex])
			{
				return ActionMapInstance.Actions[ActionIndex]->TryActivateAction();
			}
		}
	}
	return false;
}

bool UCrysActionManagerComponent::FindOrCreateActionAndTryActivateOnce(const TSubclassOf<UCrysAction> ActionClass)
{
	if (ActionClass)
	{
		UCrysAction* Action = FindActionByClass(ActionClass);
		if (!Action)
		{
			Action = CreateActionInternal(ActionClass);
			ActionPool[CacheIndex] = Action;
			CacheIndex++;
			if (CacheIndex >= MaxCacheSize)
			{
				CacheIndex = 0;
			}
		}
		return Action->TryActivateAction();
	}
	return false;
}

void UCrysActionManagerComponent::SetAction(const FGameplayTag InputTag, int32 ActionIndex, const int32 ActionSetIndex, const TSubclassOf<UCrysAction> ActionClass)
{
	if ((!InputTag.IsValid() || ActionIndex < 0) && ActionSetIndex < 0)
	{
		// Input Tag or ActionIndex is invalid and the ActionSetIndex is invalid.
		return;
	}
	
	if (!ActionClass)
	{
		ClearAction(InputTag, ActionIndex, ActionSetIndex);
		return;
	}
	
	if (!ActionMapInstances.IsValidIndex(ActionSetIndex))
	{
		ActionMapInstances.SetNum(ActionSetIndex + 1, EAllowShrinking::No);
	}
	FCrysActionMapInstance& ActionMapInstance = ActionMapInstances[ActionSetIndex];
	
	UCrysAction* NewAction = FindActionByClass(ActionClass);
	if (!NewAction)
	{
		NewAction = CreateActionInternal(ActionClass);
	}
	
	if (InputTag.IsValid())
	{
		ActionIndex = -1;
		ActionMapInstance.InputTagActionMap.Add(InputTag, NewAction);
	}
	else
	{
		if (!ActionMapInstance.Actions.IsValidIndex(ActionIndex))
		{
			ActionMapInstance.Actions.SetNum(ActionIndex + 1, EAllowShrinking::No);
		}
		ActionMapInstance.Actions[ActionIndex] = NewAction;
	}
	OnActionMapUpdatedDelegate.Broadcast(NewAction, InputTag, ActionIndex, ActionSetIndex);
}

void UCrysActionManagerComponent::SetActionMap(const FCrysActionMap& InActionMap, const int32 ActionSetIndex)
{
	ClearActions(ActionSetIndex);
	
	if (ActionSetIndex >= 0)
	{
		for (auto Iterator = InActionMap.InputTagActionMap.CreateConstIterator(); Iterator; ++Iterator)
		{
			SetAction(Iterator.Key(), -1, ActionSetIndex, Iterator.Value());
		}
		for (int32 ActionIndex = 0; ActionIndex < InActionMap.Actions.Num(); ActionIndex++)
		{
			SetAction(FGameplayTag(), ActionIndex, ActionSetIndex, InActionMap.Actions[ActionIndex]);
		}
	}
}

void UCrysActionManagerComponent::ClearAction(const FGameplayTag InputTag, const int32 ActionIndex, const int32 ActionSetIndex)
{
	if (!ActionMapInstances.IsValidIndex(ActionSetIndex))
	{
		return;
	}
	
	FCrysActionMapInstance& ActionMapInstance = ActionMapInstances[ActionSetIndex];
	if (InputTag.IsValid())
	{
		if (ActionMapInstance.InputTagActionMap.Contains(InputTag))
		{
			ActionMapInstance.InputTagActionMap.Remove(InputTag);
			OnActionMapUpdatedDelegate.Broadcast(nullptr, InputTag, -1, ActionSetIndex);
		}
	}
	else if (ActionMapInstance.Actions.IsValidIndex(ActionIndex))
	{
		ActionMapInstance.Actions[ActionIndex] = nullptr;
		OnActionMapUpdatedDelegate.Broadcast(nullptr, FGameplayTag(), ActionIndex, ActionSetIndex);
	}
}

void UCrysActionManagerComponent::ClearActions(const int32 ActionSetIndex)
{
	if (ActionMapInstances.IsValidIndex(ActionSetIndex))
	{
		FCrysActionMapInstance& ActionMapInstance = ActionMapInstances[ActionSetIndex];
		for (auto Iterator = ActionMapInstance.InputTagActionMap.CreateIterator(); Iterator; ++Iterator)
		{
			Iterator->Value = nullptr;
			OnActionMapUpdatedDelegate.Broadcast(nullptr, Iterator->Key, -1, ActionSetIndex);
			Iterator.RemoveCurrent();
		}
		for (int32 ActionIndex = 0; ActionIndex < ActionMapInstance.Actions.Num(); ActionIndex++)
		{
			ActionMapInstance.Actions[ActionIndex] = nullptr;
			OnActionMapUpdatedDelegate.Broadcast(nullptr, FGameplayTag(), ActionIndex, ActionSetIndex);
		}
		ActionMapInstance.Actions.Empty();
	}
}

void UCrysActionManagerComponent::SetActiveActionSet(const int32 Index)
{
	if (Index >= 0 && Index != ActiveActionSetIndex)
	{
		ActiveActionSetIndex = Index;
		OnActiveActionSetChangedDelegate.Broadcast(ActiveActionSetIndex);
	}
}

void UCrysActionManagerComponent::SwitchToNextActionSet(const bool bIncrementIndex)
{
	const int32 StartingIndex = GetActiveActionSetIndex();
	if (StartingIndex == 0)
	{
		// Don't proceed if Index equals 0. Reserved for overrides.
		return;
	}
	
	const int32 MaxIndex = ActionMapInstances.Num() - 1;
	int32 NextIndex = bIncrementIndex ? StartingIndex + 1 : StartingIndex - 1;
	do
	{
		if (NextIndex > MaxIndex)
		{
			// Greater than max go back to 1.
			NextIndex = 1;
		}
		else if (NextIndex < 1)
		{
			// Less than min index, loop back to Max.
			NextIndex = MaxIndex;
		}
		else if (NextIndex == StartingIndex)
		{
			// The Index is the same. Do nothing.
			return;
		}
		else if (IsActionSetEmpty(NextIndex))
		{
			// Increment/Decrement the Index and try again.
			NextIndex += bIncrementIndex ? 1 : -1;
		}
		else
		{
			// All condition pass, we set the Index.
			SetActiveActionSet(NextIndex);
			return;
		}
	}
	while (true);
}

bool UCrysActionManagerComponent::IsActionSetEmpty(const int32 Index) const
{
	if (ActionMapInstances.IsValidIndex(Index))
	{
		const bool bActionMapEmpty = ActionMapInstances[Index].InputTagActionMap.IsEmpty();
		bool bActionsEmpty = true;
		for (const TObjectPtr<UCrysAction> Action : ActionMapInstances[Index].Actions)
		{
			if (Action)
			{
				bActionsEmpty = false;
				break;
			}
		}
		return bActionsEmpty && bActionMapEmpty;
	}
	return true;
}

void UCrysActionManagerComponent::SetInputMode(const ECrysActionInputMode InInputMode)
{
	if (InputMode != InInputMode)
	{
		SetInputModeInternal(InInputMode);
		OnInputModeChangedDelegate.Broadcast(InputMode);
	}
}

void UCrysActionManagerComponent::SetInputModeInternal(const ECrysActionInputMode InInputMode)
{
	if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		if (InputModeMappingContextMap.Contains(InInputMode))
		{
			InputSubsystem->RemoveMappingContext(*InputModeMappingContextMap.Find(InInputMode));
		}
	
		InputMode = InInputMode;
	
		if (InputModeMappingContextMap.Contains(InputMode))
		{
			InputSubsystem->AddMappingContext(*InputModeMappingContextMap.Find(InputMode), InputMappingContextPriority);
		}
		else
		{
			UE_LOG(LogCrys, Error, TEXT("The CrysActionManagerComponent [%s] is missing an InputMappingContext for InputMode [%hhd]"), *GetNameSafe(GetOwner()), InputMode);
		}
	}
}

UCrysAction* UCrysActionManagerComponent::CreateActionInternal(const TSubclassOf<UCrysAction>& ActionClass)
{
	UCrysAction* NewAction = NewObject<UCrysAction>(this, ActionClass);
	NewAction->PlayerController = PlayerController;
	NewAction->Initialize();
	return NewAction;
}

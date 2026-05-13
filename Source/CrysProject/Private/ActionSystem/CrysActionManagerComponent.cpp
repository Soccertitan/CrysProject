// Copyright Soccertitan 2026


#include "ActionSystem/CrysActionManagerComponent.h"

#include "ActionSystem/CrysAction.h"
#include "Player/CrysPlayerController.h"


UCrysActionManagerComponent::UCrysActionManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	ActionPool.SetNum(MaxCacheSize);
}

void UCrysActionManagerComponent::OnRegister()
{
	Super::OnRegister();
	PlayerController = Cast<ACrysPlayerController>(GetOwner());
	check(PlayerController);
}

UCrysAction* UCrysActionManagerComponent::FindAction(const FGameplayTag InputTag, const int32 Index) const
{
	if (InputTag.IsValid() && ActionMapInstances.IsValidIndex(Index))
	{
		if (ActionMapInstances[Index].ActionMap.Contains(InputTag))
		{
			return *ActionMapInstances[Index].ActionMap.Find(InputTag);
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
			for (const TTuple<FGameplayTag, TObjectPtr<UCrysAction>>& Pair : Map.ActionMap)
			{
				if (Pair.Value->GetClass() == ActionClass)
				{
					return Pair.Value;
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

UCrysAction* UCrysActionManagerComponent::CreateAction(const TSubclassOf<UCrysAction> ActionClass)
{
	if (ActionClass)
	{
		return InternalCreateAction(ActionClass);
	}
	return nullptr;
}

bool UCrysActionManagerComponent::CreateActionAndTryActivateOnce(const TSubclassOf<UCrysAction> ActionClass)
{
	if (ActionClass)
	{
		UCrysAction* Action = FindActionByClass(ActionClass);
		if (!Action)
		{
			Action = InternalCreateAction(ActionClass);
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

bool UCrysActionManagerComponent::TryActivateAction(const FGameplayTag InputTag)
{
	return TryActivateActionAtIndex(InputTag, ActionSetIndex);
}

bool UCrysActionManagerComponent::TryActivateActionAtIndex(const FGameplayTag InputTag, const int32 Index)
{
	if (UCrysAction* Action = FindAction(InputTag, Index))
	{
		return Action->TryActivateAction();
	}
	return false;
}

void UCrysActionManagerComponent::SetAction(const FGameplayTag InputTag, const int32 Index, const TSubclassOf<UCrysAction> ActionClass)
{
	if (!InputTag.IsValid() || Index < 0)
	{
		return;
	}
	
	if (!ActionClass)
	{
		ClearAction(InputTag, Index);
		return;
	}
	
	if (!ActionMapInstances.IsValidIndex(Index))
	{
		ActionMapInstances.SetNum(Index + 1, EAllowShrinking::No);
	}

	UCrysAction* NewAction = FindActionByClass(ActionClass);
	if (!NewAction)
	{
		NewAction = InternalCreateAction(ActionClass);
	}

	ActionMapInstances[Index].ActionMap.Add(InputTag, NewAction);
	OnActionMapUpdatedDelegate.Broadcast(NewAction, InputTag, Index);
}

void UCrysActionManagerComponent::SetActionMap(const FCrysActionMap& InActionMap, const int32 Index)
{
	ClearActionMap(Index);
	
	if (Index >= 0)
	{
		for (auto Iterator = InActionMap.ActionMap.CreateConstIterator(); Iterator; ++Iterator)
		{
			SetAction(Iterator.Key(), Index, Iterator.Value());
		}
	}
}

void UCrysActionManagerComponent::ClearAction(const FGameplayTag InputTag, const int32 Index)
{
	if (!InputTag.IsValid() || !ActionMapInstances.IsValidIndex(Index))
	{
		return;
	}

	if (ActionMapInstances[Index].ActionMap.Contains(InputTag))
	{
		ActionMapInstances[Index].ActionMap.Remove(InputTag);
		OnActionMapUpdatedDelegate.Broadcast(nullptr, InputTag, Index);
	}
}

void UCrysActionManagerComponent::ClearActionMap(const int32 Index)
{
	if (ActionMapInstances.IsValidIndex(Index))
	{
		for (auto Iterator = ActionMapInstances[Index].ActionMap.CreateIterator(); Iterator; ++Iterator)
		{
			Iterator->Value = nullptr;
			OnActionMapUpdatedDelegate.Broadcast(nullptr, Iterator->Key, Index);
			Iterator.RemoveCurrent();
		}
	}
}

void UCrysActionManagerComponent::SetActionSetIndex(const int32 Index)
{
	if (Index >= 0 && Index != ActionSetIndex)
	{
		ActionSetIndex = Index;
		OnActionSetSelectedDelegate.Broadcast(ActionSetIndex);
	}
}

void UCrysActionManagerComponent::SwitchToNextActionSet(const bool bIncrementIndex)
{
	const int32 StartingIndex = GetActionSetIndex();
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
			SetActionSetIndex(NextIndex);
			return;
		}
	}
	while (true);
}

bool UCrysActionManagerComponent::IsActionSetEmpty(const int32 Index) const
{
	if (ActionMapInstances.IsValidIndex(Index))
	{
		return ActionMapInstances[Index].ActionMap.IsEmpty();
	}
	return true;
}

UCrysAction* UCrysActionManagerComponent::InternalCreateAction(const TSubclassOf<UCrysAction>& ActionClass)
{
	UCrysAction* NewAction = NewObject<UCrysAction>(this, ActionClass);
	NewAction->PlayerController = PlayerController;
	NewAction->InitializeAction();
	return NewAction;
}

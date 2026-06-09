// Copyright Soccertitan 2026


#include "ActionSystem/UI/ActionManagerViewModel.h"

#include "CrysLogChannels.h"
#include "GameplayTagContainer.h"
#include "ActionSystem/CrysAction.h"
#include "ActionSystem/CrysActionManagerComponent.h"
#include "ActionSystem/UI/ActionSlotViewModel.h"
#include "ActionSystem/UI/ActionViewModel.h"


void UActionManagerViewModel::InitializeViewModel(APlayerController* PlayerController)
{
	Super::InitializeViewModel(PlayerController);
	
	InitActionManager(PlayerController);
}

UActionSlotViewModel* UActionManagerViewModel::FindOrCreateActionSlotViewModel(const FGameplayTag& InputTag, const int32 ActionIndex, const int32 ActionSetIndex)
{
	if ((InputTag.IsValid() || ActionIndex >= 0) && ActionSetIndex >= 0)
	{
		if (!ActionViewModelContainers.IsValidIndex(ActionSetIndex))
		{
			ActionViewModelContainers.SetNum(ActionSetIndex + 1, EAllowShrinking::No);
		}
		
		FActionViewModelContainer& ActionViewModelContainer = ActionViewModelContainers[ActionSetIndex];
		return FindOrCreateActionSlotViewModelInternal(ActionViewModelContainer, InputTag, ActionIndex, ActionSetIndex);
	}
	return nullptr;
}

UActionSlotViewModel* UActionManagerViewModel::FindOrCreateActiveActionSlotViewModel(const FGameplayTag& InputTag, const int32 ActionIndex)
{
	if (InputTag.IsValid() || ActionIndex >= 0)
	{
		return FindOrCreateActionSlotViewModelInternal(ActiveActionViewModelContainer, InputTag, ActionIndex, ActiveActionSetIndex);
	}
	return nullptr;
}

void UActionManagerViewModel::InitActionManager(APlayerController* PlayerController)
{
	if (ActionManagerComponent)
	{
		return;
	}

	ActionManagerComponent = PlayerController->FindComponentByClass<UCrysActionManagerComponent>();
	if (!ActionManagerComponent)
	{
		UE_LOG(LogCrys, Error, TEXT("%s does not have a CrysActionManagerComponent"), *GetNameSafe(PlayerController));
		return;
	}

	SetActiveActionSetIndex(ActionManagerComponent->GetActiveActionSetIndex());
	SetInputMode(ActionManagerComponent->GetInputMode());
	ActionManagerComponent->OnActionMapUpdatedDelegate.AddUniqueDynamic(this, &UActionManagerViewModel::OnActionMapUpdated);
	ActionManagerComponent->OnActiveActionSetChangedDelegate.AddUniqueDynamic(this, &UActionManagerViewModel::SetActiveActionSetIndex);
	ActionManagerComponent->OnInputModeChangedDelegate.AddUniqueDynamic(this, &UActionManagerViewModel::SetInputMode);
}

void UActionManagerViewModel::SetActiveActionSetIndex(const int32 InActionSetIndex)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(ActiveActionSetIndex, InActionSetIndex))
	{
		if (ActionManagerComponent)
		{
			for (UActionSlotViewModel* Item : ActiveActionViewModelContainer.InputTagActionViewModels)
			{
				Item->SetActionViewModel(CreateActionViewModelInternal(Item->InputTag, -1, InActionSetIndex));
			}
			for (int32 ActionIndex = 0; ActiveActionViewModelContainer.ActionViewModels.Num() > ActionIndex; ActionIndex++)
			{
				ActiveActionViewModelContainer.ActionViewModels[ActionIndex]->SetActionViewModel(CreateActionViewModelInternal(FGameplayTag(), ActionIndex, InActionSetIndex));
			}
		}
	}
}

void UActionManagerViewModel::SetInputMode(ECrysActionInputMode Value)
{
	UE_MVVM_SET_PROPERTY_VALUE(InputMode, Value);
}

UActionSlotViewModel* UActionManagerViewModel::FindOrCreateActionSlotViewModelInternal(FActionViewModelContainer& ActionViewModelContainer, const FGameplayTag& InputTag, const int32 ActionIndex, const int32 ActionSetIndex)
{
	if (InputTag.IsValid())
	{
		for (UActionSlotViewModel* Item : ActionViewModelContainer.InputTagActionViewModels)
		{
			if (Item->InputTag == InputTag)
			{
				return Item;
			}
		}
		
		// Did not find an existing action. Make one now.
		UActionViewModel* ActionViewModel = CreateActionViewModelInternal(InputTag, -1, ActionSetIndex);
		UActionSlotViewModel* ActionSlotViewModel = NewObject<UActionSlotViewModel>(this);
		ActionSlotViewModel->InputTag = InputTag;
		ActionSlotViewModel->SetActionViewModel(ActionViewModel);
		ActionViewModelContainer.InputTagActionViewModels.Add(ActionSlotViewModel);
		return ActionSlotViewModel;
	}
	else
	{
		if (ActionViewModelContainer.ActionViewModels.IsValidIndex(ActionIndex))
		{
			return ActionViewModelContainer.ActionViewModels[ActionIndex];
		}
		
		// Did not find an existing action. Make one now.
		ActionViewModelContainer.ActionViewModels.SetNum(ActionIndex + 1, EAllowShrinking::No);
		UActionViewModel* ActionViewModel = CreateActionViewModelInternal(FGameplayTag(), ActionIndex, ActionSetIndex);
		UActionSlotViewModel* ActionSlotViewModel = NewObject<UActionSlotViewModel>(this);
		ActionSlotViewModel->SetActionViewModel(ActionViewModel);
		ActionViewModelContainer.ActionViewModels[ActionIndex] = ActionSlotViewModel;
		return ActionSlotViewModel;
	}
}

UActionViewModel* UActionManagerViewModel::CreateActionViewModelInternal(const FGameplayTag& InputTag, const int32 ActionIndex, int32 ActionSetIndex)
{
	if (ActionManagerComponent)
	{
		if (UCrysAction* Action = ActionManagerComponent->FindAction(InputTag, ActionIndex, ActionSetIndex))
		{
			UActionViewModel* NewVM = NewObject<UActionViewModel>(this, Action->GetActionViewModel());
			NewVM->SetAction(Action);
			return NewVM;
		}
	}
	return NewObject<UActionViewModel>(this);;
}

void UActionManagerViewModel::OnActionMapUpdated(UCrysAction* Action, const FGameplayTag& InputTag, const int32 ActionIndex, const int32 ActionSetIndex)
{
	if (ActiveActionSetIndex == ActionSetIndex)
	{
		UpdateActionViewModelInternal(ActiveActionViewModelContainer, InputTag, ActionIndex, ActionSetIndex);
	}
	
	if (ActionViewModelContainers.IsValidIndex(ActionSetIndex))
	{
		UpdateActionViewModelInternal(ActionViewModelContainers[ActionSetIndex], InputTag, ActionIndex, ActionSetIndex);
	}
}

void UActionManagerViewModel::UpdateActionViewModelInternal(FActionViewModelContainer& ActionViewModelContainer, const FGameplayTag& InputTag, const int32 ActionIndex, const int32 ActionSetIndex)
{
	if (InputTag.IsValid())
	{
		for (UActionSlotViewModel* Slot : ActionViewModelContainer.InputTagActionViewModels)
		{
			if (Slot->InputTag == InputTag)
			{
				Slot->SetActionViewModel(CreateActionViewModelInternal(InputTag, ActionIndex, ActionSetIndex));
				break;
			}
		}
	}
	else if (ActionViewModelContainer.ActionViewModels.IsValidIndex(ActionIndex))
	{
		ActionViewModelContainer.ActionViewModels[ActionIndex]->SetActionViewModel(CreateActionViewModelInternal(InputTag, ActionIndex, ActionSetIndex));
	}
}

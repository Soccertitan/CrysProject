// Copyright Soccertitan 2026


#include "ActionSystem/UI/ActionManagerViewModel.h"

#include "CrysLogChannels.h"
#include "GameplayTagContainer.h"
#include "ActionSystem/CrysAction.h"
#include "ActionSystem/CrysActionManagerComponent.h"
#include "ActionSystem/UI/ActionSlotViewModel.h"
#include "ActionSystem/UI/ActionViewModel.h"
#include "System/CrysAssetManager.h"


void UActionManagerViewModel::InitializeViewModel(APlayerController* PlayerController)
{
	Super::InitializeViewModel(PlayerController);
	
	InitActionManager(PlayerController);
}

UActionSlotViewModel* UActionManagerViewModel::FindOrCreateActionSlotViewModel(const FGameplayTag& InputTag, int32 Index)
{
	if (InputTag.IsValid() && Index >= 0)
	{
		if (!ActionSlotViewModelContainers.IsValidIndex(Index))
		{
			ActionSlotViewModelContainers.SetNum(Index + 1, EAllowShrinking::No);
		}
		
		for (UActionSlotViewModel* Item : ActionSlotViewModelContainers[Index].Items)
		{
			if (Item->InputTag == InputTag)
			{
				return Item;
			}
		}
		
		UActionViewModel* ActionViewModel = InternalCreateActionViewModel(InputTag, Index);
		UActionSlotViewModel* ActionSlotViewModel = NewObject<UActionSlotViewModel>(this);
		ActionSlotViewModel->InputTag = InputTag;
		ActionSlotViewModel->SetActionViewModel(ActionViewModel);
		ActionSlotViewModelContainers[Index].Items.Add(ActionSlotViewModel);
		return ActionSlotViewModel;
	}
	return nullptr;
}

UActionSlotViewModel* UActionManagerViewModel::FindOrCreateActiveActionSlotViewModel(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		for (UActionSlotViewModel* Item : ActiveActionSlotViewModels)
		{
			if (Item->InputTag == InputTag)
			{
				return Item;
			}
		}
		
		UActionViewModel* ActionViewModel = InternalCreateActionViewModel(InputTag, ActiveActionSetIndex);
		UActionSlotViewModel* ActionSlotViewModel = NewObject<UActionSlotViewModel>(this);
		ActionSlotViewModel->InputTag = InputTag;
		ActionSlotViewModel->SetActionViewModel(ActionViewModel);
		ActiveActionSlotViewModels.Add(ActionSlotViewModel);
		return ActionSlotViewModel;
	}
	return nullptr;
}

void UActionManagerViewModel::SetAction(const FGameplayTag& InputTag, const int32 Index, const TSoftClassPtr<UCrysAction> ActionClass)
{
	if (ActionManagerComponent && !ActionClass.IsNull())
	{
		TSubclassOf<UCrysAction> CrysActionClass = ActionClass.Get();
		if (!CrysActionClass)
		{
			CrysActionClass = UCrysAssetManager::GetSubclass(ActionClass, false);
		}
		if (CrysActionClass)
		{
			ActionManagerComponent->SetAction(InputTag, Index, CrysActionClass);
		}
	}
}

void UActionManagerViewModel::ClearAction(const FGameplayTag& InputTag, const int32 Index)
{
	if (ActionManagerComponent)
	{
		ActionManagerComponent->ClearAction(InputTag, Index);
	}
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

	SetActiveActionSetIndex(ActionManagerComponent->GetActionSetIndex());
	ActionManagerComponent->OnActionMapUpdatedDelegate.AddUniqueDynamic(this, &UActionManagerViewModel::OnActionMapUpdated);
	ActionManagerComponent->OnActionSetSelectedDelegate.AddUniqueDynamic(this, &UActionManagerViewModel::SetActiveActionSetIndex);
}

void UActionManagerViewModel::SetActiveActionSetIndex(int32 Index)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(ActiveActionSetIndex, Index))
	{
		if (ActionManagerComponent)
		{
			for (UActionSlotViewModel* Item : ActiveActionSlotViewModels)
			{
				Item->SetActionViewModel(InternalCreateActionViewModel(Item->InputTag, Index));
			}
		}
	}
}

UActionViewModel* UActionManagerViewModel::InternalCreateActionViewModel(const FGameplayTag& InputTag, int32 Index)
{
	UActionViewModel* NewVM = nullptr;
	if (ActionManagerComponent)
	{
		if (UCrysAction* Action = ActionManagerComponent->FindAction(InputTag, Index))
		{
			NewVM = NewObject<UActionViewModel>(this, Action->GetActionViewModel());
			NewVM->SetAction(Action);
		}
		else
		{
			NewVM = NewObject<UActionViewModel>(this);
		}
	}
	return NewVM;
}

void UActionManagerViewModel::OnActionMapUpdated(UCrysAction* Action, const FGameplayTag& InputTag, int32 Index)
{
	UActionViewModel* NewVM = InternalCreateActionViewModel(InputTag, Index);
	if (ActiveActionSetIndex == Index)
	{
		for (UActionSlotViewModel* Item : ActiveActionSlotViewModels)
		{
			if (Item->InputTag == InputTag)
			{
				Item->SetActionViewModel(NewVM);
				break;
			}
		}
	}
	
	if (ActionSlotViewModelContainers.IsValidIndex(Index))
	{
		for (UActionSlotViewModel* Item : ActionSlotViewModelContainers[Index].Items)
		{
			if (Item->InputTag == InputTag)
			{
				Item->SetActionViewModel(NewVM);
				break;
			}
		}
	}
}

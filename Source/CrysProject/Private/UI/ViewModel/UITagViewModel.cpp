// Copyright Soccertitan 2026


#include "UI/ViewModel/UITagViewModel.h"

#include "CrysBlueprintFunctionLibrary.h"

void UUITagViewModel::SetGameplayTag(const FGameplayTag& InValue)
{
	if (UITagInfo.Tag != InValue)
	{
		UITagInfo = UCrysBlueprintFunctionLibrary::FindUITagInfo(InValue, true);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetTagName);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetShortName);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetDescription);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetIcon);
	}
}

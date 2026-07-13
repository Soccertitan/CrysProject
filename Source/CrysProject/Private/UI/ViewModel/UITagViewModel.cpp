// Copyright Soccertitan 2026


#include "UI/ViewModel/UITagViewModel.h"

#include "CrysBlueprintFunctionLibrary.h"
#include "UI/GameplayTagInfoFragment_UI.h"

void UUITagViewModel::SetGameplayTag(const FGameplayTag& InValue)
{
	if (GameplayTag != InValue)
	{
		GameplayTag = InValue;
		if (const FCrysGameplayTagInfo* TagInfo = UCrysBlueprintFunctionLibrary::FindCrysGameplayTagInfo(GameplayTag, true))
		{
			const FGameplayTagInfoFragment_UI* FragmentPtr = TagInfo->FindFragmentByType<FGameplayTagInfoFragment_UI>();
			Fragment_UI = FragmentPtr ? *FragmentPtr : FGameplayTagInfoFragment_UI();
		}
		else
		{
			Fragment_UI = FGameplayTagInfoFragment_UI();
		}
		
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetTagName);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetShortName);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetDescription);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetIcon);
	}
}

FText UUITagViewModel::GetTagName() const
{
	return Fragment_UI.Name;
}

FText UUITagViewModel::GetShortName() const
{
	return Fragment_UI.ShortName;
}

FText UUITagViewModel::GetDescription() const
{
	return Fragment_UI.Description;
}

TSoftObjectPtr<UTexture2D> UUITagViewModel::GetIcon() const
{
	return Fragment_UI.Icon;
}

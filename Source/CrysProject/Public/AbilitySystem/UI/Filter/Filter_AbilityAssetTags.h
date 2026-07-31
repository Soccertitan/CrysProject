// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Filter/CrimFilter.h"
#include "Filter_AbilityAssetTags.generated.h"

/**
 * Filters AbilityViewModels that has any of the specified AssetTags.
 */
UCLASS()
class CRYSPROJECT_API UFilter_AbilityAssetTags : public UCrimFilter
{
	GENERATED_BODY()
	
public:
	// The ability has any of these tags.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTagContainer AbilityAssetTags;
	
	virtual bool ShouldFilterObject_Implementation(const UObject* Object, const UObject* Context) const override;
};

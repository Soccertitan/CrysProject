// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "JobContainer.generated.h"

class UJobDefinition;
/**
 * A simple asset that contains a collection of jobs. Used to have a reusable way to define which classes are allowed to do certain things.
 */
UCLASS()
class CRYSPROJECT_API UJobContainer : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TSoftObjectPtr<UJobDefinition>> Jobs;
	
	/** Auto generated when this asset is saved. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FText JobText;
	
	static FTextFormat TextFormat;
	
#if WITH_EDITORONLY_DATA
	virtual void PreSave(FObjectPreSaveContext SaveContext) override;
	
protected:
	void UpdateJobText();
#endif
};

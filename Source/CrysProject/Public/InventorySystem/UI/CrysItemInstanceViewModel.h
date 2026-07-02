// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "UI/ViewModel/ItemInstanceViewModel.h"
#include "CrysItemInstanceViewModel.generated.h"

/**
 * The base ItemInstanceViewModel for this project.
 */
UCLASS()
class CRYSPROJECT_API UCrysItemInstanceViewModel : public UItemInstanceViewModel
{
	GENERATED_BODY()
	
public:
	int32 GetUpgradeLevel() const { return UpgradeLevel; }
	bool IsEquipped() const { return bEquipped;}
	
protected:
	void SetUpgradeLevel(int32 Value);
	void SetIsEquipped(bool bValue);
	
	virtual void OnItemSet_Implementation(const TInstancedStruct<FItem>& Item) override;
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, FieldNotify, Getter, meta = (AllowPrivateAccess = "true"))
	int32 UpgradeLevel = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, FieldNotify, Getter=IsEquipped, meta = (AllowPrivateAccess = "true"))
	bool bEquipped = false;
};

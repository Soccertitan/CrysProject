// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "EquipmentItemInstanceViewModel.h"
#include "ScalableFloat.h"
#include "WeaponItemInstanceViewModel.generated.h"

/**
 * 
 */
UCLASS()
class CRYSPROJECT_API UWeaponItemInstanceViewModel : public UEquipmentItemInstanceViewModel
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, FieldNotify)
	int32 GetDamage() const {return Damage.GetValueAtLevel(GetUpgradeLevel());}
	UFUNCTION(BlueprintPure, FieldNotify)
	float GetAutoAttackDelay() const {return AutoAttackDelay.GetValueAtLevel(GetUpgradeLevel());}
	UUITagViewModel* GetWeaponSkillViewModel() const {return WeaponSkillViewModel;}
	UUITagViewModel* GetDamageTypeViewModel() const {return DamageTypeViewModel;}
	
protected:
	void SetDamage(FScalableFloat Value);
	void SetAutoAttackDelay(FScalableFloat Value);
	void SetWeaponSkillViewModel(UUITagViewModel* Value);
	void SetDamageTypeViewModel(UUITagViewModel* Value);
	
	virtual void OnItemSet_Implementation(const TInstancedStruct<FItem>& Item) override;
	virtual void OnItemDefinitionSet_Implementation(const UItemDefinition* ItemDefinition) override;
	
private:
	UPROPERTY(EditAnywhere)
	FScalableFloat Damage;
	
	UPROPERTY(EditAnywhere)
	FScalableFloat AutoAttackDelay;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, FieldNotify, Getter, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UUITagViewModel> WeaponSkillViewModel;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, FieldNotify, Getter, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UUITagViewModel> DamageTypeViewModel;
};

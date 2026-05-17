// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "EquipmentItemInstanceViewModel.h"
#include "ScalableFloat.h"
#include "AbilitySystem/Ability/Combat/CombatTypes.h"
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
	int32 GetDamage() const {return Weapon.GetDamage();}
	UFUNCTION(BlueprintPure, FieldNotify)
	float GetAutoAttackDelay() const {return Weapon.GetAutoAttackDelay();}
	UUITagViewModel* GetWeaponSkillViewModel() const {return WeaponSkillViewModel;}
	UUITagViewModel* GetDamageTypeViewModel() const {return DamageTypeViewModel;}
	
protected:
	void SetWeapon(const FCrysWeapon& Value);
	void SetWeaponLevel(int32 Level);
	void SetWeaponSkillViewModel(UUITagViewModel* Value);
	void SetDamageTypeViewModel(UUITagViewModel* Value);
	
	virtual void OnItemSet_Implementation(const TInstancedStruct<FItem>& Item) override;
	virtual void OnItemDefinitionSet_Implementation(const UItemDefinition* ItemDefinition) override;
	
private:
	UPROPERTY()
	FCrysWeapon Weapon;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, FieldNotify, Getter, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UUITagViewModel> WeaponSkillViewModel;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, FieldNotify, Getter, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UUITagViewModel> DamageTypeViewModel;
};

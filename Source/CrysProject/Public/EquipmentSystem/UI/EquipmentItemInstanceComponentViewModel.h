// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/Combat/CombatTypes.h"
#include "UI/ViewModel/Component/ItemInstanceComponentViewModel.h"
#include "EquipmentItemInstanceComponentViewModel.generated.h"

class UUITagViewModel;
/**
 * Viewmodel to display equipment data.
 */
UCLASS()
class CRYSPROJECT_API UEquipmentItemInstanceComponentViewModel : public UItemInstanceComponentViewModel
{
	GENERATED_BODY()
	
public:
	int32 GetLevelRequirement() const {return LevelRequirement;}
	
	UFUNCTION(BlueprintPure, FieldNotify)
	TArray<UUITagViewModel*> GetAllowedJobViewModels() const {return AllowedJobViewModels;}
	UFUNCTION(BlueprintPure, FieldNotify)
	UUITagViewModel* GetEquipSlotViewModels() const {return EquipSlotViewModel;}
	
	bool IsWeapon() const { return bWeapon; }
	
	UFUNCTION(BlueprintPure, FieldNotify)
	int32 GetDamage() const {return Weapon.GetDamage();}
	UFUNCTION(BlueprintPure, FieldNotify)
	float GetAutoAttackDelay() const {return Weapon.GetAutoAttackDelay();}
	UUITagViewModel* GetWeaponSkillViewModel() const {return WeaponSkillViewModel;}
	UUITagViewModel* GetDamageTypeViewModel() const {return DamageTypeViewModel;}
	
protected:
	void SetLevelRequirement(int32 Value);
	void SetAllowedJobViewModels(TArray<UUITagViewModel*> Value);
	void SetEquipSlotViewModels(UUITagViewModel* Value);
	
	void SetIsWeapon(bool bValue);
	void SetWeapon(const FCrysWeapon& Value);
	void SetWeaponLevel(int32 Level);
	void SetWeaponSkillViewModel(UUITagViewModel* Value);
	void SetDamageTypeViewModel(UUITagViewModel* Value);
	
	virtual void OnItemSet_Implementation(const TInstancedStruct<FItem>& Item) override;
	virtual void OnItemDefinitionSet_Implementation(const UItemDefinition* ItemDefinition) override;
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, FieldNotify, Getter, meta = (AllowPrivateAccess = "true"))
	int32 LevelRequirement = 0;
	
	UPROPERTY()
	TObjectPtr<UUITagViewModel> EquipSlotViewModel;
	
	UPROPERTY()
	TArray<TObjectPtr<UUITagViewModel>> AllowedJobViewModels;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, FieldNotify, Getter=IsWeapon, meta = (AllowPrivateAccess = "true"))
	bool bWeapon = false;
	
	UPROPERTY()
	FCrysWeapon Weapon;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, FieldNotify, Getter, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UUITagViewModel> WeaponSkillViewModel;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, FieldNotify, Getter, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UUITagViewModel> DamageTypeViewModel;
};

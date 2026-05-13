// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "CombatTypes.h"
#include "CrimAbilitySystemInterface.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

struct FGameplayTag;
struct FOnAttributeChangeData;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCombatAutoAttackingSignature, bool, bAutoAttacking);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCombatWeaponChangedSignature, const FCrysWeapon&, Weapon);

/**
 * Handles basic combat functions like auto attacks, weapons, and auto attack delay.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CRYSPROJECT_API UCombatComponent : public UActorComponent, public ICrimAbilitySystemInterface
{
	GENERATED_BODY()

public:
	UCombatComponent();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;
	virtual void PreNetReceive() override;
	
	virtual void SetCrimAbilitySystem_Implementation(UCrimAbilitySystemComponent* NewAbilitySystemComponent) override;
	
	UPROPERTY(BlueprintAssignable, DisplayName = "OnAutoAttackStateChanged")
	FCombatAutoAttackingSignature OnAutoAttackStateChangedDelegate;
	
	/** Starts the auto attack timer. */
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void StartAutoAttack();

	/** Ends the auto attack timer. */
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void StopAutoAttack();
	
	/** If AutoAttacking, clears the timer handle and starts a new one. */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Combat")
	void RestartAutoAttackTimer();
	
	UFUNCTION(BlueprintPure, Category = "Combat")
	bool IsAutoAttacking() const;
	
	// Returns true if AutoAttacking is allowed.
	UFUNCTION(BlueprintPure, Category = "Combat")
	bool CanAutoAttack() const;
	
	UFUNCTION(BlueprintPure, Category = "Combat")
	int32 GetBonusAttacks(const FCrysWeapon& Weapon) const;
	
	UFUNCTION(BlueprintPure, Category = "Combat")
	const FCrysWeapon& GetPrimaryWeapon() const;
	UFUNCTION(BlueprintPure, Category = "Combat")
	const FCrysWeapon& GetSecondaryWeapon() const;
	
	/** [Client + Server] Called whenever the weapon changes. */
	UPROPERTY(BlueprintAssignable, DisplayName = "OnPrimaryWeaponChanged")
	FCombatWeaponChangedSignature OnPrimaryWeaponChangedDelegate;
	/** [Client + Server] Called whenever the weapon changes. */
	UPROPERTY(BlueprintAssignable, DisplayName = "OnSecondaryWeaponChanged")
	FCombatWeaponChangedSignature OnSecondaryWeaponChangedDelegate;
	
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Combat")
	void SetPrimaryWeaponOverride(const FCrysWeapon& Weapon);
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Combat")
	void SetSecondaryWeaponOverride(const FCrysWeapon& Weapon);
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Combat")
	void ClearPrimaryWeaponOverride();
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Combat")
	void ClearSecondaryWeaponOverride();

	UFUNCTION(BlueprintPure, Category = "Combat")
	bool HasAuthority() const;
	
protected:
	/** Will use the MultiAttackChance Attribute to get the chance to strike. Index 0 has top priority. */
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TArray<FMultiAttackPriority> MultiAttackPriorities;
	
	virtual void OnRegister() override;
	
	UFUNCTION()
	void OnRep_AutoAttacking();
	
	UFUNCTION()
	void OnRep_PrimaryWeapon();
	UFUNCTION()
	void OnRep_SecondaryWeapon();
	
	UFUNCTION()
	void OnWeaponChanged(const FCrysWeapon& Weapon);
	
private:
	/** This weapon is considered the primary and is held in the "main hand". */
	UPROPERTY(EditDefaultsOnly, ReplicatedUsing=OnRep_PrimaryWeapon, Category = "Combat|Weapon")
	FCrysWeaponContainer PrimaryWeapon;
	/** The secondary weapon that is used when dual wielding. */
	UPROPERTY(EditDefaultsOnly, ReplicatedUsing=OnRep_SecondaryWeapon, Category = "Combat|Weapon")
	FCrysWeaponContainer SecondaryWeapon;
	
	UPROPERTY()
	TObjectPtr<UCrimAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY(ReplicatedUsing=OnRep_AutoAttacking)
	bool bAutoAttacking = false;
	
	FTimerHandle AutoAttackTimer;
	
	/** The time in seconds between auto attacks. */
	float AutoAttackDelay = 1.f;
	
	bool bAutoAttackTimerPaused = false;
	
	bool bCombatStance = false;
	bool bAlive = true;
	
	/** Cached value of whether the owner is a simulated actor. */
	bool bCachedIsNetSimulated = false;
	void CacheIsNetSimulated();
	
	UFUNCTION()
	void OnAutoAttackTimerCompleted();
	
	/** True after the AutoAttackTimer is completed and is reset to false when all auto attacks have completed. */
	bool bActivatingAutoAttacks = false;
	
	void OnAutoAttackDelayAttributeChanged(const FOnAttributeChangeData& Data);
	void OnLevelAttributeChanged(const FOnAttributeChangeData& Data);
	
	void OnPauseAutoAttackTagChanged(const FGameplayTag Tag, int32 NewCount);
	void OnCombatStanceTagChanged(const FGameplayTag Tag, int32 NewCount);
	void OnDeathTagChanged(const FGameplayTag Tag, int32 NewCount);
	void OnDualWieldingTagCountUpdated(const FGameplayTag Tag, int32 NewCount);
	bool bDualWielding = false;
	
	void UpdateBaseAutoAttackDelay() const;
	
	void SetWeaponOverrideInternal(FCrysWeaponContainer& WeaponContainer, const FCrysWeapon& Weapon, const FCombatWeaponChangedSignature& Delegate);
	void ClearWeaponOverrideInternal(FCrysWeaponContainer& WeaponContainer, const FCombatWeaponChangedSignature& Delegate);
	
	UFUNCTION(Server, Reliable)
	void Server_StartAutoAttack();
	
	UFUNCTION(Server, Reliable)
	void Server_StopAutoAttack();
};

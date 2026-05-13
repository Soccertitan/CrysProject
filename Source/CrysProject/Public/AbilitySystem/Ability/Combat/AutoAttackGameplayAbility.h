// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "CombatGameplayAbility.h"
#include "AutoAttackGameplayAbility.generated.h"

struct FCrysWeapon;
struct FAutoAttackParams;
/**
 * Listens to an event from the CombatComponent to trigger auto attacks. When the ability ends, will start the timer
 * for the next AutoAttack.
 */
UCLASS(Abstract)
class CRYSPROJECT_API UAutoAttackGameplayAbility : public UCombatGameplayAbility
{
	GENERATED_BODY()
	
public:
	UAutoAttackGameplayAbility();
	
	/** Use this function instead of the basic ActivateAbility for BP. All the relevant data will be extracted and ready. */
	UFUNCTION(BlueprintImplementableEvent)
	void ActivateAutoAttack(const AActor* TargetedActor, const FAutoAttackParams& AutoAttackParams);
	
	//TODO: Add functionality to get a random attack montage.
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Ability")
	void AttackTarget(const FCrysWeapon& Weapon, AActor* Target);

private:
	
};

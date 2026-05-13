// Copyright Soccertitan 2026


#include "AbilitySystem/Ability/Combat/AutoAttackGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "CrimGameplayAbilityTargetData.h"
#include "CrysNativeGameplayTags.h"
#include "AbilitySystem/Ability/Combat/CombatSystemComponent.h"
#include "Settings/CrysGameData.h"
#include "System/CrysAssetManager.h"

UAutoAttackGameplayAbility::UAutoAttackGameplayAbility()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated;
	ActivationPolicy = EAbilityActivationPolicy::OnEvent;
	
	if (HasAnyFlags(RF_ClassDefaultObject))
	{
		// Add the ability trigger tag as default to the CDO.
		FAbilityTriggerData TriggerData;
		TriggerData.TriggerTag = Crys::NativeGameplayTag::Ability_GameplayEvent_AutoAttack;
		TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
		AbilityTriggers.Add(TriggerData);
	}
}

void UAutoAttackGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if (TriggerEventData)
	{
		if (TriggerEventData->TargetData.Data.IsValidIndex(0))
		{
			FCrimGameplayAbilityTargetData* TargetData = static_cast<FCrimGameplayAbilityTargetData*>(TriggerEventData->TargetData.Data[0].Get());
			if (TargetData)
			{
				const FAutoAttackParams* AutoAttackParams = TargetData->FindCustomDataFragment<FAutoAttackParams>();
				if (AutoAttackParams)
				{
					ActivateAutoAttack(TriggerEventData->Target, *AutoAttackParams);
				}
			}
		}
	}
}

void UAutoAttackGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	
	if (GetCombatComponent())
	{
		GetCombatComponent()->OnAutoAttackCompleted(bWasCancelled);
	}
}

void UAutoAttackGameplayAbility::AttackTarget(const FCrysWeapon& Weapon, AActor* Target)
{
	if (!Target)
	{
		return;
	}
	
	TSubclassOf<UGameplayEffect> DamageEffect = Weapon.DamageGameplayEffect;
	if (!DamageEffect)
	{
		DamageEffect = UCrysAssetManager::GetSubclass(GetDefault<UCrysGameData>()->AutoAttackGameplayEffectClass);
	}
	
	if (!DamageEffect)
	{
		return;
	}
	
	if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target))
	{
		FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffect, Weapon.Level);
		SpecHandle.Data.Get()->AddDynamicAssetTag(Weapon.WeaponSkill);
		SpecHandle.Data.Get()->SetSetByCallerMagnitude(Crys::NativeGameplayTag::SetByCaller_BasePotency, 
			Weapon.Damage.GetValueAtLevel(Weapon.Level));
		
		TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
}

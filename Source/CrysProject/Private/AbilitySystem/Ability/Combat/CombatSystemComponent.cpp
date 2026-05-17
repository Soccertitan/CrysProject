// Copyright Soccertitan 2026


#include "AbilitySystem/Ability/Combat/CombatSystemComponent.h"

#include "CrimAbilityNativeGameplayTags.h"
#include "CrimAbilitySystemComponent.h"
#include "CrimGameplayAbilityTargetData.h"
#include "CrysNativeGameplayTags.h"
#include "AbilitySystem/AttributeSet/AttackerAttributeSet.h"
#include "AbilitySystem/AttributeSet/PrimaryAttributeSet.h"
#include "Net/UnrealNetwork.h"


UCombatSystemComponent::UCombatSystemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
	
	MultiAttackPriorities.Add(FMultiAttackPriority(Crys::NativeGameplayTag::Attribute_MultiAttackChance_Quadruple, 3));
	MultiAttackPriorities.Add(FMultiAttackPriority(Crys::NativeGameplayTag::Attribute_MultiAttackChance_Triple, 2));
	MultiAttackPriorities.Add(FMultiAttackPriority(Crys::NativeGameplayTag::Attribute_MultiAttackChance_Double, 1));
}

void UCombatSystemComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	FDoRepLifetimeParams Params;
	Params.bIsPushBased = true;
	Params.Condition = COND_OwnerOnly;
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, bAutoAttacking, Params);
	
	Params.Condition = COND_None;
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, PrimaryWeapon, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, SecondaryWeapon, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, AutoAttackTarget, Params);
}

void UCombatSystemComponent::BeginPlay()
{
	Super::BeginPlay();
	CacheIsNetSimulated();
}

void UCombatSystemComponent::PreNetReceive()
{
	Super::PreNetReceive();
	CacheIsNetSimulated();
}

void UCombatSystemComponent::SetCrimAbilitySystem_Implementation(UCrimAbilitySystemComponent* NewAbilitySystemComponent)
{
	if (AbilitySystemComponent == NewAbilitySystemComponent)
	{
		return;
	}
	
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UPrimaryAttributeSet::GetLevelAttribute()).RemoveAll(this);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UAttackerAttributeSet::GetAutoAttackDelayAttribute()).RemoveAll(this);
		
		AbilitySystemComponent->RegisterGameplayTagEvent(Crys::NativeGameplayTag::Ability_State_AutoAttackPaused, EGameplayTagEventType::NewOrRemoved).RemoveAll(this);
		AbilitySystemComponent->RegisterGameplayTagEvent(Crys::NativeGameplayTag::Ability_State_CombatStance, EGameplayTagEventType::NewOrRemoved).RemoveAll(this);
		AbilitySystemComponent->RegisterGameplayTagEvent(CrimAbility::NativeGameplayTag::Ability_State_Death, EGameplayTagEventType::NewOrRemoved).RemoveAll(this);
		AbilitySystemComponent->RegisterGameplayTagEvent(Crys::NativeGameplayTag::Ability_State_DualWielding, EGameplayTagEventType::NewOrRemoved).RemoveAll(this);
	}
	
	AbilitySystemComponent = NewAbilitySystemComponent;
	
	if (AbilitySystemComponent)
	{
		bool bFound = false;
		const float Value = AbilitySystemComponent->GetGameplayAttributeValue(UAttackerAttributeSet::GetAutoAttackDelayAttribute(), bFound);
		AutoAttackDelay = bFound ? Value : 1.f;
		
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UAttackerAttributeSet::GetAutoAttackDelayAttribute()).
			AddUObject(this, &UCombatSystemComponent::OnAutoAttackDelayAttributeChanged);
		
		const FGameplayTag& PauseAutoAttack = Crys::NativeGameplayTag::Ability_State_AutoAttackPaused;
		OnPauseAutoAttackTagChanged(PauseAutoAttack, AbilitySystemComponent->GetGameplayTagCount(PauseAutoAttack));
		AbilitySystemComponent->RegisterGameplayTagEvent(PauseAutoAttack, EGameplayTagEventType::NewOrRemoved).
			AddUObject(this, &UCombatSystemComponent::OnPauseAutoAttackTagChanged);
	
		const FGameplayTag& CombatStance = Crys::NativeGameplayTag::Ability_State_CombatStance;
		OnCombatStanceTagChanged(CombatStance, AbilitySystemComponent->GetGameplayTagCount(CombatStance));
		AbilitySystemComponent->RegisterGameplayTagEvent(CombatStance, EGameplayTagEventType::NewOrRemoved).
			AddUObject(this, &UCombatSystemComponent::OnCombatStanceTagChanged);
		
		const FGameplayTag& Death = CrimAbility::NativeGameplayTag::Ability_State_Death;
		OnDeathTagChanged(Death, AbilitySystemComponent->GetGameplayTagCount(Death));
		AbilitySystemComponent->RegisterGameplayTagEvent(Death, EGameplayTagEventType::NewOrRemoved).
			AddUObject(this, &UCombatSystemComponent::OnDeathTagChanged);
		
		const float Level = AbilitySystemComponent->GetNumericAttribute(UPrimaryAttributeSet::GetLevelAttribute());
		PrimaryWeapon.DefaultWeapon.SetLevel(Level);
		SecondaryWeapon.DefaultWeapon.SetLevel(Level);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UPrimaryAttributeSet::GetLevelAttribute()).
			AddUObject(this, &UCombatSystemComponent::OnLevelAttributeChanged);
		
		bDualWielding = AbilitySystemComponent->GetGameplayTagCount(Crys::NativeGameplayTag::Ability_State_DualWielding) > 0;
		AbilitySystemComponent->RegisterGameplayTagEvent(Crys::NativeGameplayTag::Ability_State_DualWielding).AddUObject(this, &UCombatSystemComponent::OnDualWieldingTagCountUpdated);
		UpdateBaseAutoAttackDelay();
	}
}

void UCombatSystemComponent::StartAutoAttack()
{
	if (IsAutoAttacking() || !CanAutoAttack())
	{
		return;
	}
	
	if (!HasAuthority())
	{
		Server_StartAutoAttack();
		return;
	}
	
	bAutoAttacking = true;
	OnAutoAttackStateChangedDelegate.Broadcast(bAutoAttacking);
	OnRep_AutoAttacking();

	if (AutoAttackDelay > 0.f)
	{
		GetWorld()->GetTimerManager().SetTimer(AutoAttackTimer, this, &UCombatSystemComponent::OnAutoAttackTimerCompleted, AutoAttackDelay, false);
		if (bAutoAttackTimerPaused)
		{
			GetWorld()->GetTimerManager().PauseTimer(AutoAttackTimer);
		}
	}
	else
	{
		OnAutoAttackTimerCompleted();	
	}
}

void UCombatSystemComponent::StopAutoAttack()
{
	if (!IsAutoAttacking())
	{
		return;
	}
	
	if (!HasAuthority())
	{
		Server_StopAutoAttack();
		return;
	}
	
	GetWorld()->GetTimerManager().ClearTimer(AutoAttackTimer);
	bAutoAttacking = false;
	OnAutoAttackStateChangedDelegate.Broadcast(bAutoAttacking);
	OnRep_AutoAttacking();
	
	bActivatingAutoAttacks = false;
}

void UCombatSystemComponent::RestartAutoAttackTimer()
{
	if (IsAutoAttacking() && HasAuthority() && bActivatingAutoAttacks == false)
	{
		GetWorld()->GetTimerManager().ClearTimer(AutoAttackTimer);
		
		if (AutoAttackDelay > 0.f)
		{
			GetWorld()->GetTimerManager().SetTimer(AutoAttackTimer, this, &UCombatSystemComponent::OnAutoAttackTimerCompleted, AutoAttackDelay, false);
			if (bAutoAttackTimerPaused)
			{
				GetWorld()->GetTimerManager().PauseTimer(AutoAttackTimer);
			}
		}
		else
		{
			OnAutoAttackTimerCompleted();
		}
	}
}

bool UCombatSystemComponent::IsAutoAttacking() const
{
	return bAutoAttacking;
}

bool UCombatSystemComponent::CanAutoAttack() const
{
	if (bCombatStance && bAlive && AbilitySystemComponent)
	{
		return true;
	}
	return false;
}

int32 UCombatSystemComponent::GetBonusAttacks(const FCrysWeapon& Weapon) const
{
	int32 Result = 0;
	if (AbilitySystemComponent)
	{
		for (const FMultiAttackPriority& MultiAttackPriority : MultiAttackPriorities)
		{
			FGameplayTagRequirements SourceTagRequirements;
			SourceTagRequirements.RequireTags.AddTag(MultiAttackPriority.MultiAttackTag);
			SourceTagRequirements.RequireTags.AddTag(Weapon.WeaponSkill);
			const float MultiAttackChance = AbilitySystemComponent->GetFilteredAttributeValue(UAttackerAttributeSet::GetMultiAttackChanceAttribute(), SourceTagRequirements, FGameplayTagContainer());
			
			if (MultiAttackChance > FMath::RandRange(0.f, 1.f))
			{
				Result = MultiAttackPriority.BonusAttackCount;
				break;
			}
		}
		
		if (Result == 0)
		{
			Result = Weapon.CalculateBonusAttacks();
		}
	}
	
	return Result;
}

const FCrysWeapon& UCombatSystemComponent::GetPrimaryWeapon() const
{
	return PrimaryWeapon.bUseWeaponOverride ? PrimaryWeapon.WeaponOverride : PrimaryWeapon.DefaultWeapon;
}

const FCrysWeapon& UCombatSystemComponent::GetSecondaryWeapon() const
{
	return SecondaryWeapon.bUseWeaponOverride ? SecondaryWeapon.WeaponOverride : SecondaryWeapon.DefaultWeapon;
}

void UCombatSystemComponent::SetPrimaryWeaponOverride(const FCrysWeapon& Weapon)
{
	if (HasAuthority())
	{
		SetWeaponOverrideInternal(PrimaryWeapon, Weapon, OnPrimaryWeaponChangedDelegate);
		OnRep_PrimaryWeapon();
	}
}

void UCombatSystemComponent::SetSecondaryWeaponOverride(const FCrysWeapon& Weapon)
{
	if (HasAuthority())
	{
		SetWeaponOverrideInternal(SecondaryWeapon, Weapon, OnSecondaryWeaponChangedDelegate);
		OnRep_SecondaryWeapon();
	}
}

void UCombatSystemComponent::ClearPrimaryWeaponOverride()
{
	if (HasAuthority())
	{
		ClearWeaponOverrideInternal(PrimaryWeapon, OnPrimaryWeaponChangedDelegate);
		OnRep_PrimaryWeapon();
	}
}

void UCombatSystemComponent::ClearSecondaryWeaponOverride()
{
	if (HasAuthority())
	{
		ClearWeaponOverrideInternal(SecondaryWeapon, OnSecondaryWeaponChangedDelegate);
		OnRep_SecondaryWeapon();
	}
}

void UCombatSystemComponent::SetAutoAttackTarget(AActor* NewTarget)
{
	if (AutoAttackTarget != NewTarget)
	{
		AutoAttackTarget = NewTarget;
		if (HasAuthority())
		{
			OnRep_AutoAttackTarget();
		}
		else
		{
			Server_SetAutoAttackTarget(NewTarget);
		}
		OnAutoAttackTargetChangedDelegate.Broadcast(NewTarget);
	}
}

bool UCombatSystemComponent::HasAuthority() const
{
	return !bCachedIsNetSimulated;
}

void UCombatSystemComponent::OnRegister()
{
	Super::OnRegister();
	
	CacheIsNetSimulated();
}

void UCombatSystemComponent::OnRep_AutoAttacking()
{
	OnAutoAttackStateChangedDelegate.Broadcast(bAutoAttacking);
}

void UCombatSystemComponent::OnRep_AutoAttackTarget()
{
	OnAutoAttackTargetChangedDelegate.Broadcast(AutoAttackTarget);
}

void UCombatSystemComponent::OnRep_PrimaryWeapon()
{
	OnPrimaryWeaponChangedDelegate.Broadcast(PrimaryWeapon.bUseWeaponOverride ? PrimaryWeapon.WeaponOverride : PrimaryWeapon.DefaultWeapon);
}

void UCombatSystemComponent::OnRep_SecondaryWeapon()
{
	OnSecondaryWeaponChangedDelegate.Broadcast(SecondaryWeapon.bUseWeaponOverride ? SecondaryWeapon.WeaponOverride : SecondaryWeapon.DefaultWeapon);
}

void UCombatSystemComponent::OnWeaponChanged(const FCrysWeapon& Weapon)
{
	RestartAutoAttackTimer();
}

void UCombatSystemComponent::CacheIsNetSimulated()
{
	bCachedIsNetSimulated = IsNetSimulating();
}

void UCombatSystemComponent::OnAutoAttackTimerCompleted()
{
#if WITH_SERVER_CODE
	if (AbilitySystemComponent && AutoAttackTarget)
	{
		FScopedPredictionWindow NewScopedWindow(AbilitySystemComponent, true);
		
		bActivatingAutoAttacks = true;
		
		FAutoAttackParams AutoAttackParams;
		AutoAttackParams.PrimaryWeapon = GetPrimaryWeapon();
		AutoAttackParams.PrimaryAttacks = 1 + GetBonusAttacks(AutoAttackParams.PrimaryWeapon);
		if (bDualWielding)
		{
			AutoAttackParams.SecondaryWeapon = GetSecondaryWeapon();
			AutoAttackParams.SecondaryAttacks = GetBonusAttacks(AutoAttackParams.SecondaryWeapon);
		}
		AutoAttackParams.RandomStream.Initialize(FMath::Rand());
		
		FCrimGameplayAbilityTargetData* Data = new FCrimGameplayAbilityTargetData();
		Data->AddCustomDataFragment(AutoAttackParams);
		
		FGameplayEventData Payload;
		Payload.EventTag = Crys::NativeGameplayTag::Ability_GameplayEvent_AutoAttack;
		Payload.Instigator = AbilitySystemComponent->GetAvatarActor();
		Payload.Target = AutoAttackTarget;
		Payload.TargetData.Add(Data);
		
		AbilitySystemComponent->HandleGameplayEvent(Payload.EventTag, &Payload);
	}
#endif // #if WITH_SERVER_CODE
}

void UCombatSystemComponent::OnAutoAttackCompleted(bool bWasCancelled)
{
	bActivatingAutoAttacks = false;
	RestartAutoAttackTimer();
}

void UCombatSystemComponent::OnAutoAttackDelayAttributeChanged(const FOnAttributeChangeData& Data)
{
	AutoAttackDelay = Data.NewValue;
}

void UCombatSystemComponent::OnLevelAttributeChanged(const FOnAttributeChangeData& Data)
{
	if (PrimaryWeapon.DefaultWeapon.GetLevel() != Data.NewValue)
	{
		PrimaryWeapon.DefaultWeapon.SetLevel(Data.NewValue);
		SecondaryWeapon.DefaultWeapon.SetLevel(Data.NewValue);
		
		if (PrimaryWeapon.bUseWeaponOverride == false ||
			(bDualWielding == true && SecondaryWeapon.bUseWeaponOverride == false))
		{
			UpdateBaseAutoAttackDelay();
		}
	}
}

void UCombatSystemComponent::OnPauseAutoAttackTagChanged(const FGameplayTag Tag, int32 NewCount)
{
	if (NewCount > 0)
	{
		if (HasAuthority())
		{
			GetWorld()->GetTimerManager().PauseTimer(AutoAttackTimer);
		}
		bAutoAttackTimerPaused = true;
	}
	else
	{
		if (HasAuthority())
		{
			GetWorld()->GetTimerManager().UnPauseTimer(AutoAttackTimer);
		}
		bAutoAttackTimerPaused = false;
	}
}

void UCombatSystemComponent::OnCombatStanceTagChanged(const FGameplayTag Tag, int32 NewCount)
{
	bCombatStance = NewCount > 0;
	if (!bCombatStance)
	{
		if (HasAuthority())
		{
			StopAutoAttack();
		}
	}
}

void UCombatSystemComponent::OnDeathTagChanged(const FGameplayTag Tag, int32 NewCount)
{
	bAlive = NewCount == 0;
	if (!bAlive)
	{
		if (HasAuthority())
		{
			StopAutoAttack();
		}
	}
}

void UCombatSystemComponent::OnDualWieldingTagCountUpdated(const FGameplayTag Tag, int32 NewCount)
{
	if (bDualWielding != NewCount > 0)
	{
		bDualWielding = NewCount > 0;
		UpdateBaseAutoAttackDelay();
	}
}

void UCombatSystemComponent::UpdateBaseAutoAttackDelay() const
{
	if (!HasAuthority())
	{
		return;
	}
	
	float TotalDelay = 0.f;
	if (PrimaryWeapon.bUseWeaponOverride)
	{
		TotalDelay = PrimaryWeapon.WeaponOverride.GetAutoAttackDelay();
	}
	else
	{
		TotalDelay = PrimaryWeapon.DefaultWeapon.GetAutoAttackDelay();
	}
	
	if (bDualWielding)
	{
		if (SecondaryWeapon.bUseWeaponOverride)
		{
			TotalDelay += SecondaryWeapon.WeaponOverride.GetAutoAttackDelay();
		}
		else
		{
			TotalDelay += SecondaryWeapon.DefaultWeapon.GetAutoAttackDelay();
		}
	}
	
	UGameplayEffect* AttackDelayGE = NewObject<UGameplayEffect>(GetTransientPackage(), FName(TEXT("BaseAutoAttackDelay")));
	AttackDelayGE->DurationPolicy = EGameplayEffectDurationType::Instant;

	int32 Idx = AttackDelayGE->Modifiers.Num();
	AttackDelayGE->Modifiers.SetNum(Idx + 1);

	FGameplayModifierInfo& InfoMaxHP = AttackDelayGE->Modifiers[Idx];
	InfoMaxHP.ModifierMagnitude = FScalableFloat(TotalDelay);
	InfoMaxHP.ModifierOp = EGameplayModOp::Override;
	InfoMaxHP.Attribute = UAttackerAttributeSet::GetAutoAttackDelayAttribute();

	AbilitySystemComponent->ApplyGameplayEffectToSelf(AttackDelayGE, 1.0f, AbilitySystemComponent->MakeEffectContext());
}

void UCombatSystemComponent::SetWeaponOverrideInternal(FCrysWeaponContainer& WeaponContainer, const FCrysWeapon& Weapon,
	const FCombatWeaponChangedSignature& Delegate)
{
	WeaponContainer.bUseWeaponOverride = true;
	WeaponContainer.WeaponOverride = Weapon;
	
	if (AbilitySystemComponent)
	{
		UpdateBaseAutoAttackDelay();
	}
	
	RestartAutoAttackTimer();
	Delegate.Broadcast(Weapon);
}

void UCombatSystemComponent::ClearWeaponOverrideInternal(FCrysWeaponContainer& WeaponContainer, const FCombatWeaponChangedSignature& Delegate)
{
	WeaponContainer.bUseWeaponOverride = false;
	WeaponContainer.WeaponOverride = FCrysWeapon();
	
	if (AbilitySystemComponent)
	{
		UpdateBaseAutoAttackDelay();
	}
	
	RestartAutoAttackTimer();
	Delegate.Broadcast(WeaponContainer.DefaultWeapon);
}

void UCombatSystemComponent::Server_SetAutoAttackTarget_Implementation(AActor* NewTarget)
{
	SetAutoAttackTarget(NewTarget);
}

void UCombatSystemComponent::Server_StartAutoAttack_Implementation()
{
	StartAutoAttack();
}

void UCombatSystemComponent::Server_StopAutoAttack_Implementation()
{
	StopAutoAttack();
}

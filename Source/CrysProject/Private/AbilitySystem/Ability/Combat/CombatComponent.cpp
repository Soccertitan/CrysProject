// Copyright Soccertitan 2026


#include "AbilitySystem/Ability/Combat/CombatComponent.h"

#include "CrimAbilityNativeGameplayTags.h"
#include "CrimAbilitySystemComponent.h"
#include "CrimGameplayAbilityTargetData.h"
#include "CrysNativeGameplayTags.h"
#include "AbilitySystem/AttributeSet/AttackerAttributeSet.h"
#include "AbilitySystem/AttributeSet/PrimaryAttributeSet.h"
#include "Net/UnrealNetwork.h"


UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
	
	MultiAttackPriorities.Add(FMultiAttackPriority(Crys::NativeGameplayTag::Attribute_MultiAttackChance_Quadruple, 3));
	MultiAttackPriorities.Add(FMultiAttackPriority(Crys::NativeGameplayTag::Attribute_MultiAttackChance_Triple, 2));
	MultiAttackPriorities.Add(FMultiAttackPriority(Crys::NativeGameplayTag::Attribute_MultiAttackChance_Double, 1));
}

void UCombatComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	FDoRepLifetimeParams Params;
	Params.bIsPushBased = true;
	Params.Condition = COND_OwnerOnly;
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, bAutoAttacking, Params);
	
	Params.Condition = COND_None;
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, PrimaryWeapon, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, SecondaryWeapon, Params);
}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	CacheIsNetSimulated();
}

void UCombatComponent::PreNetReceive()
{
	Super::PreNetReceive();
	CacheIsNetSimulated();
}

void UCombatComponent::SetCrimAbilitySystem_Implementation(UCrimAbilitySystemComponent* NewAbilitySystemComponent)
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
			AddUObject(this, &UCombatComponent::OnAutoAttackDelayAttributeChanged);
		
		const FGameplayTag& PauseAutoAttack = Crys::NativeGameplayTag::Ability_State_AutoAttackPaused;
		OnPauseAutoAttackTagChanged(PauseAutoAttack, AbilitySystemComponent->GetGameplayTagCount(PauseAutoAttack));
		AbilitySystemComponent->RegisterGameplayTagEvent(PauseAutoAttack, EGameplayTagEventType::NewOrRemoved).
			AddUObject(this, &UCombatComponent::OnPauseAutoAttackTagChanged);
	
		const FGameplayTag& CombatStance = Crys::NativeGameplayTag::Ability_State_CombatStance;
		OnCombatStanceTagChanged(CombatStance, AbilitySystemComponent->GetGameplayTagCount(CombatStance));
		AbilitySystemComponent->RegisterGameplayTagEvent(CombatStance, EGameplayTagEventType::NewOrRemoved).
			AddUObject(this, &UCombatComponent::OnCombatStanceTagChanged);
		
		const FGameplayTag& Death = CrimAbility::NativeGameplayTag::Ability_State_Death;
		OnDeathTagChanged(Death, AbilitySystemComponent->GetGameplayTagCount(Death));
		AbilitySystemComponent->RegisterGameplayTagEvent(Death, EGameplayTagEventType::NewOrRemoved).
			AddUObject(this, &UCombatComponent::OnDeathTagChanged);
		
		const float Level = AbilitySystemComponent->GetNumericAttribute(UPrimaryAttributeSet::GetLevelAttribute());
		PrimaryWeapon.DefaultWeapon.Level = Level;
		SecondaryWeapon.DefaultWeapon.Level = Level;
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UPrimaryAttributeSet::GetLevelAttribute()).
			AddUObject(this, &UCombatComponent::OnLevelAttributeChanged);
		
		bDualWielding = AbilitySystemComponent->GetGameplayTagCount(Crys::NativeGameplayTag::Ability_State_DualWielding) > 0;
		AbilitySystemComponent->RegisterGameplayTagEvent(Crys::NativeGameplayTag::Ability_State_DualWielding).AddUObject(this, &UCombatComponent::OnDualWieldingTagCountUpdated);
		UpdateBaseAutoAttackDelay();
	}
}

void UCombatComponent::StartAutoAttack()
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
		GetWorld()->GetTimerManager().SetTimer(AutoAttackTimer, this, &UCombatComponent::OnAutoAttackTimerCompleted, AutoAttackDelay, false);
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

void UCombatComponent::StopAutoAttack()
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
	
	//TODO: Clear the AutoAttackAbility Queue.
	bActivatingAutoAttacks = false;
}

void UCombatComponent::RestartAutoAttackTimer()
{
	if (IsAutoAttacking() && HasAuthority() && bActivatingAutoAttacks == false)
	{
		GetWorld()->GetTimerManager().ClearTimer(AutoAttackTimer);
		
		if (AutoAttackDelay > 0.f)
		{
			GetWorld()->GetTimerManager().SetTimer(AutoAttackTimer, this, &UCombatComponent::OnAutoAttackTimerCompleted, AutoAttackDelay, false);
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

bool UCombatComponent::IsAutoAttacking() const
{
	return bAutoAttacking;
}

bool UCombatComponent::CanAutoAttack() const
{
	if (bCombatStance && bAlive && AbilitySystemComponent)
	{
		return true;
	}
	return false;
}

int32 UCombatComponent::GetBonusAttacks(const FCrysWeapon& Weapon) const
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
			const float MultiAttackChance = Weapon.MultiAttackChance.GetValueAtLevel(Weapon.Level);
			 
			if (MultiAttackChance > 0.f && MultiAttackChance >= FMath::RandRange(0.f, 1.f))
			{
				float TotalProbability = 0;
				for (const float& Probability : Weapon.MultiAttackDistribution)
				{
					TotalProbability += Probability;
				}
				
				float HitValue = FMath::FRandRange(0.f, TotalProbability);
				float RunningValue = 0.f;
				for (int32 Index = 0; Index < Weapon.MultiAttackDistribution.Num(); Index++)
				{
					// Count up until we find the first item that exceeds the HitValue.
					RunningValue += Weapon.MultiAttackDistribution[Index];
					if (HitValue <= RunningValue)
					{
						Result = Weapon.MultiAttackDistribution[Index] + 1;
						break;
					}
				}
			}
		}
	}
	
	return Result;
}

const FCrysWeapon& UCombatComponent::GetPrimaryWeapon() const
{
	return PrimaryWeapon.bUseWeaponOverride ? PrimaryWeapon.WeaponOverride : PrimaryWeapon.DefaultWeapon;
}

const FCrysWeapon& UCombatComponent::GetSecondaryWeapon() const
{
	return SecondaryWeapon.bUseWeaponOverride ? SecondaryWeapon.WeaponOverride : SecondaryWeapon.DefaultWeapon;
}

void UCombatComponent::SetPrimaryWeaponOverride(const FCrysWeapon& Weapon)
{
	if (HasAuthority())
	{
		SetWeaponOverrideInternal(PrimaryWeapon, Weapon, OnPrimaryWeaponChangedDelegate);
		OnRep_PrimaryWeapon();
	}
}

void UCombatComponent::SetSecondaryWeaponOverride(const FCrysWeapon& Weapon)
{
	if (HasAuthority())
	{
		SetWeaponOverrideInternal(SecondaryWeapon, Weapon, OnSecondaryWeaponChangedDelegate);
		OnRep_SecondaryWeapon();
	}
}

void UCombatComponent::ClearPrimaryWeaponOverride()
{
	if (HasAuthority())
	{
		ClearWeaponOverrideInternal(PrimaryWeapon, OnPrimaryWeaponChangedDelegate);
		OnRep_PrimaryWeapon();
	}
}

void UCombatComponent::ClearSecondaryWeaponOverride()
{
	if (HasAuthority())
	{
		ClearWeaponOverrideInternal(SecondaryWeapon, OnSecondaryWeaponChangedDelegate);
		OnRep_SecondaryWeapon();
	}
}

bool UCombatComponent::HasAuthority() const
{
	return !bCachedIsNetSimulated;
}

void UCombatComponent::OnRegister()
{
	Super::OnRegister();
	
	CacheIsNetSimulated();
}

void UCombatComponent::OnRep_AutoAttacking()
{
	OnAutoAttackStateChangedDelegate.Broadcast(bAutoAttacking);
}

void UCombatComponent::OnRep_PrimaryWeapon()
{
	OnPrimaryWeaponChangedDelegate.Broadcast(PrimaryWeapon.bUseWeaponOverride ? PrimaryWeapon.WeaponOverride : PrimaryWeapon.DefaultWeapon);
}

void UCombatComponent::OnRep_SecondaryWeapon()
{
	OnSecondaryWeaponChangedDelegate.Broadcast(SecondaryWeapon.bUseWeaponOverride ? SecondaryWeapon.WeaponOverride : SecondaryWeapon.DefaultWeapon);
}

void UCombatComponent::OnWeaponChanged(const FCrysWeapon& Weapon)
{
	RestartAutoAttackTimer();
}

void UCombatComponent::CacheIsNetSimulated()
{
	bCachedIsNetSimulated = IsNetSimulating();
}

void UCombatComponent::OnAutoAttackTimerCompleted()
{
#if WITH_SERVER_CODE
	if (AbilitySystemComponent)
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
		Payload.Target = nullptr; //TODO Get Target UCrysBlueprintFunctionLibrary::GetAbilityTarget(AbilitySystemComponent->GetOwner(), Crys::NativeGameplayTag::Ability_GameplayEvent_AutoAttack.GetTag().GetSingleTagContainer());
		Payload.TargetData.Add(Data);
		
		AbilitySystemComponent->HandleGameplayEvent(Payload.EventTag, &Payload);
	}
#endif // #if WITH_SERVER_CODE
}

void UCombatComponent::OnAutoAttackDelayAttributeChanged(const FOnAttributeChangeData& Data)
{
	AutoAttackDelay = Data.NewValue;
}

void UCombatComponent::OnLevelAttributeChanged(const FOnAttributeChangeData& Data)
{
	if (PrimaryWeapon.DefaultWeapon.Level != Data.NewValue)
	{
		PrimaryWeapon.DefaultWeapon.Level = Data.NewValue;
		SecondaryWeapon.DefaultWeapon.Level = Data.NewValue;
		
		if (PrimaryWeapon.bUseWeaponOverride == false ||
			(bDualWielding == true && SecondaryWeapon.bUseWeaponOverride == false))
		{
			UpdateBaseAutoAttackDelay();
		}
	}
}

void UCombatComponent::OnPauseAutoAttackTagChanged(const FGameplayTag Tag, int32 NewCount)
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

void UCombatComponent::OnCombatStanceTagChanged(const FGameplayTag Tag, int32 NewCount)
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

void UCombatComponent::OnDeathTagChanged(const FGameplayTag Tag, int32 NewCount)
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

void UCombatComponent::OnDualWieldingTagCountUpdated(const FGameplayTag Tag, int32 NewCount)
{
	if (bDualWielding != NewCount > 0)
	{
		bDualWielding = NewCount > 0;
		UpdateBaseAutoAttackDelay();
	}
}

void UCombatComponent::UpdateBaseAutoAttackDelay() const
{
	if (!HasAuthority())
	{
		return;
	}
	
	float TotalDelay = 0.f;
	if (PrimaryWeapon.bUseWeaponOverride)
	{
		TotalDelay = PrimaryWeapon.WeaponOverride.AutoAttackDelay.GetValueAtLevel(PrimaryWeapon.WeaponOverride.Level);
	}
	else
	{
		TotalDelay = PrimaryWeapon.DefaultWeapon.AutoAttackDelay.GetValueAtLevel(PrimaryWeapon.DefaultWeapon.Level);
	}
	
	if (bDualWielding)
	{
		if (SecondaryWeapon.bUseWeaponOverride)
		{
			TotalDelay += SecondaryWeapon.WeaponOverride.AutoAttackDelay.GetValueAtLevel(SecondaryWeapon.WeaponOverride.Level);
		}
		else
		{
			TotalDelay += SecondaryWeapon.DefaultWeapon.AutoAttackDelay.GetValueAtLevel(SecondaryWeapon.DefaultWeapon.Level);
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

void UCombatComponent::SetWeaponOverrideInternal(FCrysWeaponContainer& WeaponContainer, const FCrysWeapon& Weapon,
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

void UCombatComponent::ClearWeaponOverrideInternal(FCrysWeaponContainer& WeaponContainer, const FCombatWeaponChangedSignature& Delegate)
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

void UCombatComponent::Server_StartAutoAttack_Implementation()
{
	StartAutoAttack();
}

void UCombatComponent::Server_StopAutoAttack_Implementation()
{
	StopAutoAttack();
}

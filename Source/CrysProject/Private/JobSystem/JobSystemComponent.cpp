// Copyright Soccertitan 2026


#include "JobSystem/JobSystemComponent.h"

#include "CrimAbilitySystemComponent.h"
#include "AbilitySystem/AttributeSet/JobAttributeSet.h"
#include "AbilitySystem/AttributeSet/ManaPointsAttributeSet.h"
#include "AbilitySystem/AttributeSet/PrimaryAttributeSet.h"
#include "Attribute/HitPointsAttributeSet.h"
#include "Engine/AssetManager.h"
#include "JobSystem/JobDefinition.h"
#include "JobSystem/JobTypes.h"
#include "Net/UnrealNetwork.h"


FJobParams::FJobParams(const FJobInfo& Info)
{
	Race = Info.Race;
	RaceLevel = Info.RaceLevel;
	MainJob = Info.MainJob;
	MainJobLevel = Info.MainJobLevel;
	SubJob = Info.SubJob;
	SubJobLevel = Info.SubJobLevel;
	SubJobEffectiveness = Info.SubJobEffectiveness;
}

UJobSystemComponent::UJobSystemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UJobSystemComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Race, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, MainJob, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, SubJob, COND_None, REPNOTIFY_Always);
}

void UJobSystemComponent::BeginPlay()
{
	Super::BeginPlay();
	
	CacheIsNetSimulated();
}

void UJobSystemComponent::PreNetReceive()
{
	Super::PreNetReceive();
	
	CacheIsNetSimulated();
}

void UJobSystemComponent::SetJobs(FJobParams JobParams)
{
	if (!HasAuthority())
	{
		return;
	}
	
	bChangingJobs = true;
	OnChangingJobsDelegate.Broadcast(bChangingJobs);
	
	if (Race != JobParams.Race)
	{
		Race = JobParams.Race;
		OnRaceChangedDelegate.Broadcast(Race);
		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, Race, this);
		OnRep_Race();
	}
	if (MainJob != JobParams.MainJob)
	{
		MainJob = JobParams.MainJob;
		OnMainJobChangedDelegate.Broadcast(MainJob);
		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, MainJob, this);
		OnRep_MainJob();
	}
	if (SubJob != JobParams.SubJob)
	{
		SubJob = JobParams.SubJob;
		OnSubJobChangedDelegate.Broadcast(SubJob);
		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, SubJob, this);
		OnRep_SubJob();
	}
	
	if (AbilitySystemComponent)
	{
		OverrideBaseAttribute(JobParams.RaceLevel, UPrimaryAttributeSet::GetLevelAttribute());
		OverrideBaseAttribute(JobParams.MainJobLevel, UJobAttributeSet::GetMainJobLevelAttribute());
		OverrideBaseAttribute(JobParams.SubJobLevel, UJobAttributeSet::GetSubJobLevelAttribute());
		OverrideBaseAttribute(JobParams.SubJobEffectiveness, UJobAttributeSet::GetSubJobEffectivenessAttribute());
		
		ApplyBaseAttributes();
		
		RemoveActiveGameplayEffects(RaceActiveGameplayEffectHandles);
		RemoveActiveGameplayEffects(MainJobActiveGameplayEffectHandles);
		RemoveActiveGameplayEffects(SubJobActiveGameplayEffectHandles);
		
		GrantGameplayEffects(Race, JobParams.RaceLevel, RaceActiveGameplayEffectHandles);
		GrantGameplayEffects(MainJob, JobParams.MainJobLevel, MainJobActiveGameplayEffectHandles);
		GrantGameplayEffects(SubJob, JobParams.SubJobLevel, SubJobActiveGameplayEffectHandles);
		
		if (JobParams.bMaximizeHpMp)
		{
			MaximizeHpMpAttributes();
		}
	}
	
	bChangingJobs = false;
	OnChangingJobsDelegate.Broadcast(bChangingJobs);
}

FJobInfo UJobSystemComponent::GetJobs() const
{
	FJobInfo Info;
	Info.Race = Race;
	Info.MainJob = MainJob;
	Info.SubJob = SubJob;
	
	if (AbilitySystemComponent)
	{
		Info.RaceLevel = AbilitySystemComponent->GetNumericAttributeBase(UPrimaryAttributeSet::GetLevelAttribute());
		Info.MainJobLevel = AbilitySystemComponent->GetNumericAttributeBase(UJobAttributeSet::GetMainJobLevelAttribute());
		Info.SubJobLevel = AbilitySystemComponent->GetNumericAttributeBase(UJobAttributeSet::GetSubJobLevelAttribute());
		Info.SubJobEffectiveness = AbilitySystemComponent->GetNumericAttributeBase(UJobAttributeSet::GetSubJobEffectivenessAttribute());
	}
	
	return Info;
}

void UJobSystemComponent::SetCrimAbilitySystem_Implementation(UCrimAbilitySystemComponent* InAbilitySystemComponent)
{
	if (AbilitySystemComponent == InAbilitySystemComponent)
	{
		return;
	}
	
	if (AbilitySystemComponent)
	{
		RemoveBindingToAttributeDelegates();
		
		RemoveActiveGameplayEffects(RaceActiveGameplayEffectHandles);
		RemoveActiveGameplayEffects(MainJobActiveGameplayEffectHandles);
		RemoveActiveGameplayEffects(SubJobActiveGameplayEffectHandles);
	}
	
	AbilitySystemComponent = InAbilitySystemComponent;
	
	if (HasAuthority())
	{
		BindToAttributeDelegates();
		ApplyBaseAttributes();
		GrantGameplayEffects(Race, AbilitySystemComponent->GetNumericAttributeBase(UPrimaryAttributeSet::GetLevelAttribute()), RaceActiveGameplayEffectHandles);
		GrantGameplayEffects(MainJob, AbilitySystemComponent->GetNumericAttributeBase(UJobAttributeSet::GetMainJobLevelAttribute()), MainJobActiveGameplayEffectHandles);
		GrantGameplayEffects(SubJob, AbilitySystemComponent->GetNumericAttributeBase(UJobAttributeSet::GetSubJobLevelAttribute()), SubJobActiveGameplayEffectHandles);
		MaximizeHpMpAttributes();
	}
}

bool UJobSystemComponent::HasAuthority() const
{
	return !bCachedIsNetSimulated;
}

void UJobSystemComponent::OnRep_Race()
{
	OnRaceChangedDelegate.Broadcast(Race);
}

void UJobSystemComponent::OnRep_MainJob()
{
	OnMainJobChangedDelegate.Broadcast(MainJob);
}

void UJobSystemComponent::OnRep_SubJob()
{
	OnSubJobChangedDelegate.Broadcast(SubJob);
}

void UJobSystemComponent::RemoveBindingToAttributeDelegates()
{
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		UPrimaryAttributeSet::GetLevelAttribute()).RemoveAll(this);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		UJobAttributeSet::GetMainJobLevelAttribute()).RemoveAll(this);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		UJobAttributeSet::GetSubJobLevelAttribute()).RemoveAll(this);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		UJobAttributeSet::GetSubJobEffectivenessAttribute()).RemoveAll(this);
}

void UJobSystemComponent::BindToAttributeDelegates()
{
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		UPrimaryAttributeSet::GetLevelAttribute()).AddUObject(this, &UJobSystemComponent::OnAttributeChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		UJobAttributeSet::GetMainJobLevelAttribute()).AddUObject(this, &UJobSystemComponent::OnAttributeChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		UJobAttributeSet::GetSubJobLevelAttribute()).AddUObject(this, &UJobSystemComponent::OnAttributeChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		UJobAttributeSet::GetSubJobEffectivenessAttribute()).AddUObject(this, &UJobSystemComponent::OnAttributeChanged);
}

void UJobSystemComponent::OnAttributeChanged(const FOnAttributeChangeData& Data)
{
	if (!bChangingJobs)
	{
		ApplyBaseAttributes();
	}
}

void UJobSystemComponent::OverrideBaseAttribute(const float Value, const FGameplayAttribute& Attribute)
{
	if (HasAuthority() && AbilitySystemComponent)
	{
		UGameplayEffect* InstantGE = NewObject<UGameplayEffect>(GetTransientPackage(), FName(TEXT("InstantGE")));
		InstantGE->DurationPolicy = EGameplayEffectDurationType::Instant;

		int32 Idx = InstantGE->Modifiers.Num();
		InstantGE->Modifiers.SetNum(Idx + 1);

		FGameplayModifierInfo& ModifierInfo = InstantGE->Modifiers[Idx];
		ModifierInfo.ModifierMagnitude = FScalableFloat(Value);
		ModifierInfo.ModifierOp = EGameplayModOp::Override;
		ModifierInfo.Attribute = Attribute;
		AbilitySystemComponent->ApplyGameplayEffectToSelf(InstantGE, 1.0f, AbilitySystemComponent->MakeEffectContext());
	}
}

void UJobSystemComponent::ApplyBaseAttributes() const
{
	UGameplayEffect* BaseStats = NewObject<UGameplayEffect>(GetTransientPackage(), FName(TEXT("BaseStats")));
	BaseStats->DurationPolicy = EGameplayEffectDurationType::Instant;
	FPrimaryAttributesCalc CalculatedAttributes(
		Race, AbilitySystemComponent->GetNumericAttribute(UPrimaryAttributeSet::GetLevelAttribute()), 
		MainJob, AbilitySystemComponent->GetNumericAttribute(UJobAttributeSet::GetMainJobLevelAttribute()),
		SubJob, AbilitySystemComponent->GetNumericAttribute(UJobAttributeSet::GetSubJobLevelAttribute()),
		AbilitySystemComponent->GetNumericAttribute(UJobAttributeSet::GetSubJobEffectivenessAttribute()));
	
	int32 Idx = BaseStats->Modifiers.Num();
	BaseStats->Modifiers.SetNum(Idx + 9);
	
	FGameplayModifierInfo& InfoMaxHP = BaseStats->Modifiers[Idx];
	InfoMaxHP.ModifierMagnitude = FScalableFloat(CalculatedAttributes.MaxHitPoints);
	InfoMaxHP.ModifierOp = EGameplayModOp::Override;
	InfoMaxHP.Attribute = UHitPointsAttributeSet::GetMaxPointsAttribute();
	
	FGameplayModifierInfo& InfoMaxMP = BaseStats->Modifiers[Idx + 1];
	InfoMaxMP.ModifierMagnitude = FScalableFloat(CalculatedAttributes.MaxMagicPoints);
	InfoMaxMP.ModifierOp = EGameplayModOp::Override;
	InfoMaxMP.Attribute = UManaPointsAttributeSet::GetMaxPointsAttribute();
	
	FGameplayModifierInfo& InfoStrength = BaseStats->Modifiers[Idx + 2];
	InfoStrength.ModifierMagnitude = FScalableFloat(CalculatedAttributes.Strength);
	InfoStrength.ModifierOp = EGameplayModOp::Override;
	InfoStrength.Attribute = UPrimaryAttributeSet::GetStrengthAttribute();
	
	FGameplayModifierInfo& InfoVitality = BaseStats->Modifiers[Idx + 3];
	InfoVitality.ModifierMagnitude = FScalableFloat(CalculatedAttributes.Vitality);
	InfoVitality.ModifierOp = EGameplayModOp::Override;
	InfoVitality.Attribute = UPrimaryAttributeSet::GetVitalityAttribute();
	
	FGameplayModifierInfo& InfoDexterity = BaseStats->Modifiers[Idx +4 ];
	InfoDexterity.ModifierMagnitude = FScalableFloat(CalculatedAttributes.Dexterity);
	InfoDexterity.ModifierOp = EGameplayModOp::Override;
	InfoDexterity.Attribute = UPrimaryAttributeSet::GetDexterityAttribute();
	
	FGameplayModifierInfo& InfoAgility = BaseStats->Modifiers[Idx + 5];
	InfoAgility.ModifierMagnitude = FScalableFloat(CalculatedAttributes.Agility);
	InfoAgility.ModifierOp = EGameplayModOp::Override;
	InfoAgility.Attribute = UPrimaryAttributeSet::GetAgilityAttribute();
	
	FGameplayModifierInfo& InfoIntelligence = BaseStats->Modifiers[Idx + 6];
	InfoIntelligence.ModifierMagnitude = FScalableFloat(CalculatedAttributes.Intelligence);
	InfoIntelligence.ModifierOp = EGameplayModOp::Override;
	InfoIntelligence.Attribute = UPrimaryAttributeSet::GetIntelligenceAttribute();
	
	FGameplayModifierInfo& InfoMind = BaseStats->Modifiers[Idx + 7];
	InfoMind.ModifierMagnitude = FScalableFloat(CalculatedAttributes.Mind);
	InfoMind.ModifierOp = EGameplayModOp::Override;
	InfoMind.Attribute = UPrimaryAttributeSet::GetMindAttribute();
	
	FGameplayModifierInfo& InfoCharisma = BaseStats->Modifiers[Idx + 8];
	InfoCharisma.ModifierMagnitude = FScalableFloat(CalculatedAttributes.Charisma);
	InfoCharisma.ModifierOp = EGameplayModOp::Override;
	InfoCharisma.Attribute = UPrimaryAttributeSet::GetCharismaAttribute();

	AbilitySystemComponent->ApplyGameplayEffectToSelf(BaseStats, 1.0f, AbilitySystemComponent->MakeEffectContext());
}

void UJobSystemComponent::MaximizeHpMpAttributes()
{
	UGameplayEffect* GameplayEffect = NewObject<UGameplayEffect>(GetTransientPackage(), FName(TEXT("MaximizeAttributes")));
	GameplayEffect->DurationPolicy = EGameplayEffectDurationType::Instant;
	
	int32 Idx = GameplayEffect->Modifiers.Num();
	GameplayEffect->Modifiers.SetNum(Idx + 2);
	
	FGameplayModifierInfo& InfoMaxHP = GameplayEffect->Modifiers[Idx];
	InfoMaxHP.ModifierMagnitude = FScalableFloat(AbilitySystemComponent->GetNumericAttribute(UHitPointsAttributeSet::GetMaxPointsAttribute()));
	InfoMaxHP.ModifierOp = EGameplayModOp::Override;
	InfoMaxHP.Attribute = UHitPointsAttributeSet::GetCurrentPointsAttribute();
	
	FGameplayModifierInfo& InfoMaxMP = GameplayEffect->Modifiers[Idx + 1];
	InfoMaxMP.ModifierMagnitude = FScalableFloat(AbilitySystemComponent->GetNumericAttribute(UManaPointsAttributeSet::GetMaxPointsAttribute()));
	InfoMaxMP.ModifierOp = EGameplayModOp::Override;
	InfoMaxMP.Attribute = UManaPointsAttributeSet::GetCurrentPointsAttribute();
	
	AbilitySystemComponent->ApplyGameplayEffectToSelf(GameplayEffect, 1.0f, AbilitySystemComponent->MakeEffectContext());
}

void UJobSystemComponent::CacheIsNetSimulated()
{
	bCachedIsNetSimulated = IsNetSimulating();
}

void UJobSystemComponent::GrantGameplayEffects(const UJobDefinition* Job, const int32 Level, TArray<FActiveGameplayEffectHandle>& OutHandles)
{
	if (Job)
	{
		TArray<FSoftObjectPath> GameplayEffectsToLoad;
		for (const FJobGameplayEffects& Effect : Job->GameplayEffects)
		{
			if (Level >= Effect.Level)
			{
				GameplayEffectsToLoad.Reserve(GameplayEffectsToLoad.Num() + Effect.GameplayEffects.Num());
				
				for (const TSoftClassPtr<UGameplayEffect>& EffectClass : Effect.GameplayEffects)
				{
					GameplayEffectsToLoad.Add(EffectClass.ToSoftObjectPath());
				}
			}
		}
		
		if (GameplayEffectsToLoad.Num() == 0)
		{
			return;
		}
		
		OutHandles.Reserve(GameplayEffectsToLoad.Num());
		// FStreamableDelegate DelegateToCall = FStreamableDelegate::CreateUObject(this, &UJobSystemComponent::OnGameplayEffectsLoaded, GameplayEffectsToLoad, Level, OutHandles);
		UAssetManager::Get().LoadAssetList(GameplayEffectsToLoad)->WaitUntilComplete();
		
		for (const FSoftObjectPath& Path : GameplayEffectsToLoad)
		{
			if (TSubclassOf<UGameplayEffect> GameplayEffectClass = Cast<UClass>(Path.ResolveObject()))
			{
				const UGameplayEffect* GameplayEffect = GameplayEffectClass->GetDefaultObject<UGameplayEffect>();
				const FActiveGameplayEffectHandle Handle = AbilitySystemComponent->ApplyGameplayEffectToSelf(GameplayEffect, Level, AbilitySystemComponent->MakeEffectContext());
				OutHandles.Add(Handle);
			}
		}
	}
}

void UJobSystemComponent::RemoveActiveGameplayEffects(TArray<FActiveGameplayEffectHandle>& InHandles)
{
	for (const FActiveGameplayEffectHandle& Handle : InHandles)
	{
		AbilitySystemComponent->RemoveActiveGameplayEffect(Handle, 1);
	}
	InHandles.Empty();
}

// void UJobSystemComponent::OnGameplayEffectsLoaded(TArray<FSoftObjectPath> LoadedGameplayEffectClasses, const int32 Level, TArray<FActiveGameplayEffectHandle>* OutHandles)
// {
// 	if (AbilitySystemComponent)
// 	{
// 		for (const FSoftObjectPath& Path : LoadedGameplayEffectClasses)
// 		{
// 			if (TSubclassOf<UGameplayEffect> GameplayEffectClass = Cast<UClass>(Path.ResolveObject()))
// 			{
// 				const UGameplayEffect* GameplayEffect = GameplayEffectClass->GetDefaultObject<UGameplayEffect>();
// 				const FActiveGameplayEffectHandle Handle = AbilitySystemComponent->ApplyGameplayEffectToSelf(GameplayEffect, Level, AbilitySystemComponent->MakeEffectContext());
// 				OutHandles->Add(Handle);
// 			}
// 		}
// 	}
// }

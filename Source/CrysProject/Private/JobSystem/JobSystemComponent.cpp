// Copyright Soccertitan 2026


#include "JobSystem/JobSystemComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "CrimAbilitySystemBlueprintFunctionLibrary.h"
#include "CrimAbilitySystemComponent.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystem/AttributeSet/JobAttributeSet.h"
#include "AbilitySystem/AttributeSet/ManaPointsAttributeSet.h"
#include "AbilitySystem/AttributeSet/PrimaryAttributeSet.h"
#include "Attribute/HitPointsAttributeSet.h"
#include "Engine/AssetManager.h"
#include "JobSystem/JobDefinition.h"
#include "JobSystem/JobTypes.h"
#include "Net/UnrealNetwork.h"


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
		UpdateJobTags(Race, JobParams.Race);
		Race = JobParams.Race;
		OnRaceChangedDelegate.Broadcast(Race);
		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, Race, this);
		OnRep_Race();
	}
	if (MainJob != JobParams.MainJob)
	{
		UpdateJobTags(MainJob, JobParams.MainJob);
		MainJob = JobParams.MainJob;
		OnMainJobChangedDelegate.Broadcast(MainJob);
		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, MainJob, this);
		OnRep_MainJob();
	}
	if (SubJob != JobParams.SubJob)
	{
		UpdateJobTags(SubJob, JobParams.SubJob);
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
		ApplyGameplayEffects();
		
		if (JobParams.bMaximizeHpMp)
		{
			MaximizeHpMpAttributes();
		}
	}
	
	bChangingJobs = false;
	OnChangingJobsDelegate.Broadcast(bChangingJobs);
}

FJobParams UJobSystemComponent::MakeJobParams() const
{
	FJobParams Result;
	Result.Race = Race;
	Result.MainJob = MainJob;
	Result.SubJob = SubJob;
	
	if (AbilitySystemComponent)
	{
		Result.RaceLevel = AbilitySystemComponent->GetNumericAttributeBase(UPrimaryAttributeSet::GetLevelAttribute());
		Result.MainJobLevel = AbilitySystemComponent->GetNumericAttributeBase(UJobAttributeSet::GetMainJobLevelAttribute());
		Result.SubJobLevel = AbilitySystemComponent->GetNumericAttributeBase(UJobAttributeSet::GetSubJobLevelAttribute());
		Result.SubJobEffectiveness = AbilitySystemComponent->GetNumericAttributeBase(UJobAttributeSet::GetSubJobEffectivenessAttribute());
	}
	
	return Result;
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
		ClearJobTags();
		
		RemoveActiveGameplayEffects(ActiveGameplayEffects.RaceActiveGameplayEffectHandles);
		RemoveActiveGameplayEffects(ActiveGameplayEffects.MainJobActiveGameplayEffectHandles);
		RemoveActiveGameplayEffects(ActiveGameplayEffects.SubJobActiveGameplayEffectHandles);
	}
	
	AbilitySystemComponent = InAbilitySystemComponent;
	
	if (HasAuthority())
	{
		BindToAttributeDelegates();
		SetJobTags();
		ApplyBaseAttributes();
		ApplyGameplayEffects();
		MaximizeHpMpAttributes();
	}
}

UCrimAbilitySystemComponent* UJobSystemComponent::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
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
		if (Data.NewValue != Data.OldValue)
		{
			ApplyBaseAttributes();
		}
		
		if (Data.Attribute == UPrimaryAttributeSet::GetLevelAttribute())
		{
			UpdateActiveGameplayEffects(Race, UPrimaryAttributeSet::GetLevelAttribute(), ActiveGameplayEffects.RaceActiveGameplayEffectHandles);
		}
		else if (Data.Attribute == UJobAttributeSet::GetMainJobLevelAttribute())
		{
			UpdateActiveGameplayEffects(MainJob, UJobAttributeSet::GetMainJobLevelAttribute(), ActiveGameplayEffects.MainJobActiveGameplayEffectHandles);
		}
		else if (Data.Attribute == UJobAttributeSet::GetSubJobLevelAttribute())
		{
			UpdateActiveGameplayEffects(SubJob, UJobAttributeSet::GetSubJobLevelAttribute(), ActiveGameplayEffects.SubJobActiveGameplayEffectHandles);
		}
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

void UJobSystemComponent::UpdateJobTags(const UJobDefinition* OldJob, const UJobDefinition* NewJob) const
{
	if (AbilitySystemComponent)
	{
		if (OldJob)
		{
			AbilitySystemComponent->RemoveLooseGameplayTag(OldJob->JobTag, 1, EGameplayTagReplicationState::TagOnly);
		}
		if (NewJob)
		{
			AbilitySystemComponent->AddLooseGameplayTag(NewJob->JobTag, 1, EGameplayTagReplicationState::TagOnly);
		}
	}
}

void UJobSystemComponent::SetJobTags()
{
	UpdateJobTags(nullptr, Race);
	UpdateJobTags(nullptr, MainJob);
	UpdateJobTags(nullptr, SubJob);
}

void UJobSystemComponent::ClearJobTags()
{
	UpdateJobTags(Race, nullptr);
	UpdateJobTags(MainJob, nullptr);
	UpdateJobTags(SubJob, nullptr);
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

void UJobSystemComponent::ApplyGameplayEffects()
{
	RemoveActiveGameplayEffects(ActiveGameplayEffects.RaceActiveGameplayEffectHandles);
	RemoveActiveGameplayEffects(ActiveGameplayEffects.MainJobActiveGameplayEffectHandles);
	RemoveActiveGameplayEffects(ActiveGameplayEffects.SubJobActiveGameplayEffectHandles);
		
	UpdateActiveGameplayEffects(Race, UPrimaryAttributeSet::GetLevelAttribute(), ActiveGameplayEffects.RaceActiveGameplayEffectHandles);
	UpdateActiveGameplayEffects(MainJob, UJobAttributeSet::GetMainJobLevelAttribute(), ActiveGameplayEffects.MainJobActiveGameplayEffectHandles);
	UpdateActiveGameplayEffects(SubJob, UJobAttributeSet::GetSubJobLevelAttribute(), ActiveGameplayEffects.SubJobActiveGameplayEffectHandles);
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

void UJobSystemComponent::UpdateActiveGameplayEffects(const UJobDefinition* Job, const FGameplayAttribute& Attribute, FJobSystemActiveGameplayEffectHandles& InHandles)
{
	if (Job == nullptr)
	{
		return;
	}
	
	bool bSuccess = false;
	/** Get the "base" level from channel 0. */
	int32 Level = UCrimAbilitySystemBlueprintFunctionLibrary::EvaluateAttributeValueWithTagsUpToChannel(
		AbilitySystemComponent, Attribute, EGameplayModEvaluationChannel::Channel0, FGameplayTagContainer(), FGameplayTagContainer(), bSuccess);
	
	if (InHandles.GrantedLevel == Level)
	{
		// The base level never changed from before, we skip adding/remove/update GameplayEffects.
		return;
	}
	
	InHandles.GrantedLevel = Level;
	
	for (const FJobDefinitionGameplayEffects& Effect : Job->GameplayEffectsForLevels)
	{
		if (Level >= Effect.Level)
		{
			// Grants effects or update existing levels on granted effects exist.
			if (TArray<FActiveGameplayEffectHandle>* ActiveHandles = InHandles.ActiveGameplayEffectHandlesForLevels.Find(Effect.Level))
			{
				// Have existing effects, update the level here.
				for (const FActiveGameplayEffectHandle& Handle : *ActiveHandles)
				{
					AbilitySystemComponent->SetActiveGameplayEffectLevel(Handle, Level);
				}
			}
			else
			{
				// Don't have active effects in the handle yet. Grant and add the handles now.
				TArray<FSoftObjectPath> GameplayEffectsToLoad;
				GameplayEffectsToLoad.Reserve(Effect.GameplayEffects.Num());
				TArray<FActiveGameplayEffectHandle> ActiveGameplayEffectHandles;
				ActiveGameplayEffectHandles.Reserve(Effect.GameplayEffects.Num());
				for (const TSoftClassPtr<UGameplayEffect>& EffectClass : Effect.GameplayEffects)
				{
					GameplayEffectsToLoad.Add(EffectClass.ToSoftObjectPath());
				}
				UAssetManager::Get().LoadAssetList(GameplayEffectsToLoad)->WaitUntilComplete();
				
				for (const FSoftObjectPath& Path : GameplayEffectsToLoad)
				{
					if (const TSubclassOf<UGameplayEffect> GameplayEffectClass = Cast<UClass>(Path.ResolveObject()))
					{
						const UGameplayEffect* GameplayEffect = GameplayEffectClass->GetDefaultObject<UGameplayEffect>();
						ActiveGameplayEffectHandles.Add(AbilitySystemComponent->ApplyGameplayEffectToSelf(GameplayEffect, Level, AbilitySystemComponent->MakeEffectContext()));
					}
				}
				InHandles.ActiveGameplayEffectHandlesForLevels.Add(Effect.Level, ActiveGameplayEffectHandles);
			}
		}
		else
		{
			// Remove granted effects.
			if (TArray<FActiveGameplayEffectHandle>* ActiveHandles = InHandles.ActiveGameplayEffectHandlesForLevels.Find(Effect.Level))
			{
				for (const FActiveGameplayEffectHandle& Handle : *ActiveHandles)
				{
					AbilitySystemComponent->RemoveActiveGameplayEffect(Handle, 1);
				}
				InHandles.ActiveGameplayEffectHandlesForLevels.Remove(Effect.Level);
			}
		}
	}
}

void UJobSystemComponent::RemoveActiveGameplayEffects(FJobSystemActiveGameplayEffectHandles& InHandles)
{
	for (const TTuple<int, TArray<FActiveGameplayEffectHandle>> Pair : InHandles.ActiveGameplayEffectHandlesForLevels)
	{
		for (const FActiveGameplayEffectHandle Handle : Pair.Value)
		{
			AbilitySystemComponent->RemoveActiveGameplayEffect(Handle, 1);
		}
	}
	InHandles.GrantedLevel = -1;
	InHandles.ActiveGameplayEffectHandlesForLevels.Empty();
}

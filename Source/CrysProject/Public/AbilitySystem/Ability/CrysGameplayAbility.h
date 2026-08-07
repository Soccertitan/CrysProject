// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "Ability/CrimGameplayAbility.h"
#include "AbilitySystem/AbilityTargetType.h"
#include "CrysGameplayAbility.generated.h"

/**
 * The base GameplayAbility for this project.
 */
UCLASS(Abstract)
class CRYSPROJECT_API UCrysGameplayAbility : public UCrimGameplayAbility
{
	GENERATED_BODY()
	
public:
	UCrysGameplayAbility();
	
	UFUNCTION(BlueprintPure, Category = "Ability")
	EAbilityTargetType GetTargetType() const { return TargetType; }
	
	/** Returns the TargetMaxDistance scaled at ability level. */
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "Ability")
	float GetTargetMaxDistance() const;
	
	/** Returns the TargetHalfAngle scaled at ability level. */
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "Ability")
	float GetTargetHalfAngle() const;
	
	/** Returns the CastTime scaled at ability level multiplied by the CastTime multiplier evaluated with SourceTags. */
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "Ability")
	float GetCastTime() const;
	
	virtual float GetCooldownTime() const override;
	
	UFUNCTION(BlueprintPure, Category = "Ability")
	FGameplayTagRequirements GetTargetTagRequirements() const { return TargetTagRequirements; }
	
	/** 
	 * Returns true if the target meets all requirements from the ability. Wrapper for IsTargetValid. 
	 * Distance, HalfAngle, TargetType, and TargetTagRequirements. 
	 */
	UFUNCTION(BlueprintPure, Category = "Ability")
	bool IsPrimaryTargetValid(AActor* TargetActor) const;
	
	/** Returns true if the target meets all requirements. */
	UFUNCTION(BlueprintPure, Category = "Ability")
	bool IsTargetValid(const FVector Origin, const FVector ForwardVector, AActor* TargetActor, const float MaxDistance, const float HalfAngle, const EAbilityTargetType TargetType, const FGameplayTagRequirements TargetTagRequirements) const;
	
	/** Returns true if the target meets the AbilityTargetType requirement. */
	UFUNCTION(BlueprintPure, Category = "Ability")
	bool IsTargetOfAbilityTargetType(AActor* TargetActor, const EAbilityTargetType TargetType) const;
	
	/** Returns true if the target is within range of the AvatarActor. */
	UFUNCTION(BlueprintPure, Category = "Ability")
	bool IsTargetWithinRange(AActor* TargetActor, float Range) const;
	
	/** Returns true, if the TargetActor's ASC meets the requirements. */
	UFUNCTION(BlueprintPure, Category = "Ability")
	bool DoesTargetMeetTargetTagRequirements(AActor* TargetActor, const FGameplayTagRequirements& Requirements) const;

	FText GetAbilityName() const { return AbilityName; }
	UFUNCTION(BlueprintNativeEvent)
	FText GetAbilityDescription() const;
	FText GetAbilityDescription_Implementation() const { return AbilityDescription; }
	TSoftObjectPtr<UTexture2D> GetIcon() const { return Icon; }
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	FText AbilityName;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI", meta = (MultiLine))
	FText AbilityDescription;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSoftObjectPtr<UTexture2D> Icon;
	
	/** Applies a GE to the target actor. Setting the Tag for SetByCaller values. */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Ability")
	void ApplyGameplayEffectToTarget(AActor* TargetActor, AActor* EffectCauser = nullptr);
	
private:
	/** Defines who the ability can target. */
	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	EAbilityTargetType TargetType;
	
	/** The target must be within max range to be affected. */
	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	FScalableFloat TargetMaxDistance;
	
	/** The target must be within the half angle to be affected. */
	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	FScalableFloat TargetHalfAngle;
	
	/** The time to wait before an ability effects targets. */
	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	FScalableFloat CastTime;
	
	/** The target must meet these conditions to apply a gameplay effect. */
	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	FGameplayTagRequirements TargetTagRequirements;
};

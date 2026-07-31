// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "MVVMViewModelBase.h"
#include "AbilityViewModel.generated.h"

class UGameplayAbility;
struct FGameplayTag;
struct FGameplayAbilitySpec;
class UCrimGameplayAbility;
class UCrimAbilitySystemComponent;

/**
 * Details about an ability. This only expects the AbilityData and ASC to be set once. If you need to show new data create a new ViewModel.
 */
UCLASS()
class CRYSPROJECT_API UAbilityViewModel : public UMVVMViewModelBase
{
	GENERATED_BODY()
	
public:
	FText GetAbilityName() const { return AbilityName; }
	TSoftObjectPtr<UTexture2D> GetIcon() const { return Icon; }
	bool IsAbilityGranted() const { return bAbilityGranted; }
	bool IsAbilityOnCooldown() const { return bAbilityOnCooldown; }
	
	UFUNCTION(BlueprintPure, FieldNotify, Category = "Viewmodel|Ability")
	float GetCooldownTimeRemaining() const;
	
	// Sets up the viewmodel with the required data. If using this, it will assume the ability is granted.
	void SetGameplayAbility(const FGameplayAbilitySpec& Spec, UCrimAbilitySystemComponent* AbilitySystemComponent);
	// Searches for the Ability from the ASC if possible. If it fails to find, CDO of the AbilityClass is used. 
	void SetGameplayAbility(TSubclassOf<UGameplayAbility> AbilityClass, UCrimAbilitySystemComponent* AbilitySystemComponent);
	void SetGameplayAbility(UGameplayAbility* Ability);
	
	UFUNCTION(BlueprintPure)
	const UGameplayAbility* GetGameplayAbility() const { return Ability; }
	UFUNCTION(BlueprintPure)
	TSubclassOf<UGameplayAbility> GetGameplayAbilityClass() const { return AbilityClass; }
	
protected:
	void SetAbilityName(const FText& NewValue);
	void SetIcon(const TSoftObjectPtr<UTexture2D>& NewValue);
	void SetIsAbilityGranted(const bool NewValue);
	void SetIsAbilityOnCooldown(const bool NewValue);
	
	UFUNCTION(BlueprintPure, Category = "Viewmodel|Ability")
	UCrimAbilitySystemComponent* GetAbilitySystemComponent() const { return AbilitySystemComponent; }

	virtual void OnGameplayAbilitySet(UGameplayAbility* NewAbility, UGameplayAbility* OldAbility);
	
	virtual void OnAbilitySystemComponentSet(UCrimAbilitySystemComponent* NewASC, UCrimAbilitySystemComponent* OldASC);
	
	virtual void OnAbilityGiven(const FGameplayAbilitySpec& Spec);
	virtual void OnAbilityRemoved(const FGameplayAbilitySpec& Spec);
	
	// Gets the cooldown tags from the ability.
	virtual FGameplayTagContainer GenerateCooldownTags() const;
	
private:
	// The ASC is cached to bind to delegates.
	UPROPERTY()
	TObjectPtr<UCrimAbilitySystemComponent> AbilitySystemComponent;
	// The ability CDO or the PrimaryInstance from a spec.
	UPROPERTY()
	TObjectPtr<UGameplayAbility> Ability;
	UPROPERTY()
	TSubclassOf<UGameplayAbility> AbilityClass;
	
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Category = "Viewmodel|Ability", meta = (AllowPrivateAccess = true))
	FText AbilityName;
	
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Category = "Viewmodel|Ability", meta = (AllowPrivateAccess = true))
	TSoftObjectPtr<UTexture2D> Icon;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter="IsAbilityGranted", Category = "Viewmodel|Ability", meta = (AllowPrivateAccess = true))
	bool bAbilityGranted = false;
	
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter="IsAbilityOnCooldown", Category = "Viewmodel|Ability", meta = (AllowPrivateAccess = true))
	bool bAbilityOnCooldown = false;
	FGameplayTagContainer CooldownTags;
	TMap<FGameplayTag, FDelegateHandle> BoundCooldownTagsASCHandles;
	
	void SetAbilitySystemComponent(UCrimAbilitySystemComponent* NewAbilitySystemComponent);
	
	// Binds to the cooldown tags from the ability and ASC.
	void TryBindToAbilityCooldownTags();
	void UnbindToAbilityCooldownTags(UCrimAbilitySystemComponent* ASC);
	
	void HandleCooldownTagCountChanged(const FGameplayTag GameplayTag, int32 Count);
};

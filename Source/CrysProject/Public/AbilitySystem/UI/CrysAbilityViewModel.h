// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "MVVMViewModelBase.h"
#include "CrysAbilityViewModel.generated.h"

class UCrysGameplayAbility;
struct FGameplayTag;
struct FGameplayAbilitySpec;
class UCrimGameplayAbility;
class UCrimAbilitySystemComponent;

/**
 * Details about an ability. This only expects the AbilityData and ASC to be set once. If you need to show new data create a new ViewModel.
 */
UCLASS()
class CRYSPROJECT_API UCrysAbilityViewModel : public UMVVMViewModelBase
{
	GENERATED_BODY()
	
public:
	FText GetAbilityName() const { return AbilityName; }
	FText GetAbilityDescription() const { return AbilityDescription; }
	TSoftObjectPtr<UTexture2D> GetIcon() const { return Icon; }
	bool IsAbilityGranted() const { return bAbilityGranted; }
	bool IsAbilityOnCooldown() const { return bAbilityOnCooldown; }
	
	UFUNCTION(BlueprintPure, FieldNotify, Category = "Viewmodel|Ability")
	float GetCooldownTimeRemaining() const;
	
	// Sets up the viewmodel with the required data. If using this, it will assume the ability is granted.
	void SetGameplayAbility(const FGameplayAbilitySpec& Spec, UCrimAbilitySystemComponent* AbilitySystemComponent);
	// Searches for the Ability from the ASC if possible. If it fails to find, CDO of the AbilityClass is used. 
	void SetGameplayAbility(TSubclassOf<UCrysGameplayAbility> AbilityClass, UCrimAbilitySystemComponent* AbilitySystemComponent);
	void SetGameplayAbility(UCrysGameplayAbility* Ability);
	
	UFUNCTION(BlueprintPure)
	const UCrysGameplayAbility* GetGameplayAbility() const { return Ability; }
	UFUNCTION(BlueprintPure)
	TSubclassOf<UCrysGameplayAbility> GetGameplayAbilityClass() const { return AbilityClass; }
	
protected:
	void SetAbilityName(const FText& NewValue);
	void SetAbilityDescription(const FText& NewValue);
	void SetIcon(const TSoftObjectPtr<UTexture2D>& NewValue);
	void SetIsAbilityGranted(const bool NewValue);
	void SetIsAbilityOnCooldown(const bool NewValue);
	
	UFUNCTION(BlueprintPure, Category = "Viewmodel|Ability")
	UCrimAbilitySystemComponent* GetAbilitySystemComponent() const { return AbilitySystemComponent; }

	virtual void OnGameplayAbilitySet(UCrysGameplayAbility* NewAbility, UCrysGameplayAbility* OldAbility);
	
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
	TObjectPtr<UCrysGameplayAbility> Ability;
	UPROPERTY()
	TSubclassOf<UCrysGameplayAbility> AbilityClass;
	
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Category = "Viewmodel|Ability", meta = (AllowPrivateAccess = true))
	FText AbilityName;
	
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Category = "Viewmodel|Ability", meta = (AllowPrivateAccess = true))
	FText AbilityDescription;
	
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

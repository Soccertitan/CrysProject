// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "MVVMViewModelBase.h"
#include "AbilityViewModel.generated.h"

class UGameplayAbility;
struct FGameplayTag;
struct FGameplayAbilitySpec;
class UGameplayAbilityData;
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
	
	/** Use this to have the viewmodel use live data. */
	void SetAbilitySystemComponent(UCrimAbilitySystemComponent* NewAbilitySystemComponent);
	void SetGameplayAbilityData(UGameplayAbilityData* AbilityData);
	
protected:
	void SetAbilityName(const FText& NewValue);
	void SetIcon(const TSoftObjectPtr<UTexture2D>& NewValue);
	void SetIsAbilityGranted(const bool NewValue);
	void SetIsAbilityOnCooldown(const bool NewValue);
	
	UFUNCTION(BlueprintPure, Category = "Viewmodel|Ability")
	UCrimAbilitySystemComponent* GetAbilitySystemComponent() const { return AbilitySystemComponent; }
	UFUNCTION(BlueprintPure, Category = "Viewmodel|Ability")
	UGameplayAbilityData* GetGameplayAbilityData() const { return GameplayAbilityData; }
	
	// Called whenever the ASC or AbilityData is set to a new non-null value. AbilityData is guaranteed to be valid.
	virtual void UpdateViewModelData(UGameplayAbilityData* AbilityData);
	
	virtual void OnAbilitySystemComponentSet(UCrimAbilitySystemComponent* OldAbilitySystemComponent);
	
	virtual void OnAbilityGiven(const FGameplayAbilitySpec& Spec);
	virtual void OnAbilityRemoved(const FGameplayAbilitySpec& Spec);
	
	// Gets the cooldown tags from the ability.
	virtual FGameplayTagContainer GenerateCooldownTags(UGameplayAbility* GameplayAbility) const;
	
private:
	// Can use the ASC to tailor the data displayed.
	UPROPERTY()
	TObjectPtr<UCrimAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY()
	TObjectPtr<UGameplayAbilityData> GameplayAbilityData;
	
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Category = "Viewmodel|Ability", meta = (AllowPrivateAccess = true))
	FText AbilityName;
	
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Category = "Viewmodel|Ability", meta = (AllowPrivateAccess = true))
	TSoftObjectPtr<UTexture2D> Icon;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter="IsAbilityGranted", Category = "Viewmodel|Ability", meta = (AllowPrivateAccess = true))
	bool bAbilityGranted = false;
	void UpdateIsAbilityGranted();
	
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter="IsAbilityOnCooldown", Category = "Viewmodel|Ability", meta = (AllowPrivateAccess = true))
	bool bAbilityOnCooldown = false;
	FGameplayTagContainer CooldownTags;
	TMap<FGameplayTag, FDelegateHandle> BoundCooldownTagsASCHandles;
	
	void BindToAbilityCooldownTags(const FGameplayAbilitySpec& Spec);
	void UnbindToAbilityCooldownTags(UCrimAbilitySystemComponent* ASC);
	
	void HandleCooldownTagCountChanged(const FGameplayTag GameplayTag, int32 Count);
};

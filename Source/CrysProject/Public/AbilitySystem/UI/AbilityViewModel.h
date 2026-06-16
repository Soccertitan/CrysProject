// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "AbilityViewModel.generated.h"

class UGameplayAbilityData;
class UCrimGameplayAbility;
class UCrimAbilitySystemComponent;

/**
 * Details about an ability.
 */
UCLASS()
class CRYSPROJECT_API UAbilityViewModel : public UMVVMViewModelBase
{
	GENERATED_BODY()
	
public:
	FText GetAbilityName() const { return AbilityName; }
	TSoftObjectPtr<UTexture2D> GetIcon() const { return Icon; }
	
	/** Use this to have the viewmodel use live data. */
	void SetAbilitySystemComponent(UCrimAbilitySystemComponent* NewAbilitySystemComponent);
	void SetGameplayAbilityData(UGameplayAbilityData* AbilityData);
	
protected:
	void SetAbilityName(const FText& NewValue);
	void SetIcon(const TSoftObjectPtr<UTexture2D>& NewValue);
	
	UFUNCTION(BlueprintPure, Category = "Viewmodel|Ability")
	UCrimAbilitySystemComponent* GetAbilitySystemComponent() const { return AbilitySystemComponent; }
	UFUNCTION(BlueprintPure, Category = "Viewmodel|Ability")
	UGameplayAbilityData* GetGameplayAbilityData() const { return GameplayAbilityData; }
	
	// Called whenever the ASC or AbilityData is set to new values.
	virtual void UpdateViewModelData(UGameplayAbilityData* AbilityData);
	
	virtual void OnAbilitySystemComponentSet(UCrimAbilitySystemComponent* OldAbilitySystemComponent) {}
	
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
};

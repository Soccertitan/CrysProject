// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "CrysViewModel.h"
#include "HeroOverlayViewModel.generated.h"

class UCrimTargetingSystemComponent;
struct FCrimTargetPoint;
class UCrimAbilitySystemComponent;
class UCombatSystemComponent;

/**
 * An overlay ViewModel designed for the player controlled Hero.
 */
UCLASS()
class CRYSPROJECT_API UHeroOverlayViewModel : public UCrysViewModel
{
	GENERATED_BODY()
	
public:
	bool IsRootWidgetAdded() const { return bRootWidgetAdded; }
	bool IsAutoAttacking() const { return bAutoAttacking; }
	
	virtual void InitializeViewModel(APlayerController* PlayerController) override;
	
protected:
	/** The AbilitySystemComponent from the player state. */
	UFUNCTION(BlueprintImplementableEvent)
	void OnAbilitySystemComponentRetrieved(UCrimAbilitySystemComponent* AbilitySystemComponent);
	
	/** Called anytime the target changes. */
	UFUNCTION(BlueprintImplementableEvent)
	void OnTargetPointChanged(const FCrimTargetPoint& OldTarget, const FCrimTargetPoint& NewTarget);
	
	UFUNCTION()
	void SetIsAutoAttacking(bool Value);
	
	void OnRootWidgetAdded();
	void OnRootWidgetRemoved();
	
	void SetIsRootWidgetAdded(bool Value);

	virtual void OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn) override;

private:
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter=IsRootWidgetAdded, meta = (AllowPrivateAccess = true))
	bool bRootWidgetAdded = false;
	
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter=IsAutoAttacking, meta = (AllowPrivateAccess = true))
	bool bAutoAttacking = false;
	
	UPROPERTY()
	TObjectPtr<UCombatSystemComponent> CombatSystemComponent;
	
	UPROPERTY()
	TObjectPtr<UCrimTargetingSystemComponent> TargetingSystemComponent;
};

// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "CrysPlayerState.h"
#include "GenericTeamAgentInterface.h"
#include "InventorySystemInterface.h"
#include "HeroPlayerState.generated.h"

class UCrimAbilitySystemComponent;
/**
 * All hero's should use this player state. As this will restore and save data for the player.
 */
UCLASS()
class CRYSPROJECT_API AHeroPlayerState : public ACrysPlayerState, public IAbilitySystemInterface,
	public IInventorySystemInterface, public IGenericTeamAgentInterface
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCrimAbilitySystemComponent> AbilitySystemComponent;
	
public:
	AHeroPlayerState();
	virtual void PostInitializeComponents() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	/** IAbilitySystemInterface */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
};

// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "AbilitySystem/AttributeTagRelationship.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UI/UITagRelationship.h"
#include "CrysBlueprintFunctionLibrary.generated.h"

struct FCrysGameplayTagInfo;
class UCrysViewModel;
class ACrysCharacter;
class ACrysPlayerState;
class ACrysPlayerController;

/**
 * 
 */
UCLASS()
class CRYSPROJECT_API UCrysBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	/** Returns the PlayerController, will check if it's a PlayerController or try and retrieve from the PlayerState or Pawn. */
	UFUNCTION(BlueprintPure, Category = "CrysFunctionLibrary", meta = (DefaultToSelf = Actor))
	static ACrysPlayerController* GetCrysPlayerController(AActor* Actor);

	/** Returns the PlayerState, will check if it's a PlayerState or try and retrieve from the PlayerController or Pawn. */
	UFUNCTION(BlueprintPure, Category = "CrysFunctionLibrary", meta = (DefaultToSelf = Actor))
	static ACrysPlayerState* GetCrysPlayerState(AActor* Actor);
	
	/** Returns the Character, will check if it's a Character or try and retrieve from the PlayerState or PlayerController. */
	UFUNCTION(BlueprintPure, Category = "CrysFunctionLibrary", meta = (DefaultToSelf = Actor))
	static ACrysCharacter* GetCrysCharacter(AActor* Actor);
	
	/**
	 * If a ViewModel of class already exists returns that ViewModel. Otherwise, creates a new ViewModel of
	 * the class type. Then initializes the ViewModel with the owning HUD's PlayerController.
	 */
	UFUNCTION(BlueprintCallable, Category = "CrysFunctionLibrary", meta=(DeterminesOutputType=ViewModelClass))
	static UCrysViewModel* FindOrCreateViewModel(const TSubclassOf<UCrysViewModel> ViewModelClass, APlayerController* PlayerController);
	
	// /** Returns an Actor that is targeted if the source Actor implements the AbilityTargetInterface */
	// UFUNCTION(BlueprintPure, Category = "CrysFunctionLibrary", meta = (DefaultToSelf = Actor))
	// static AActor* GetAbilityTarget(const AActor* Actor, const FGameplayTagContainer& AbilityTags);
	
	// /** Tries to retrieve the CombatTextComponent from the Actor. */
	// UFUNCTION(BlueprintPure, Category = "CrysFunctionLibrary", meta = (DefaultToSelf = Actor))
	// static UCombatTextComponent* GetCombatTextComponent(AActor* Actor);
	
	static FCrysGameplayTagInfo FindCrysGameplayTagInfo(const FGameplayTag& Tag, bool bLogNotFound = false);
	static FAttributeTagInfo FindAttributeTagInfo(const FGameplayTag& Tag, bool bLogNotFound = false);
	static FUITagInfo FindUITagInfo(const FGameplayTag& Tag, bool bLogNotFound = false);
	
	UFUNCTION(BlueprintPure, Category = "CrysFunctionLibrary")
	static ETeamAttitude::Type GetAttitudeTowardsActor(AActor* Source, AActor* Target);
};

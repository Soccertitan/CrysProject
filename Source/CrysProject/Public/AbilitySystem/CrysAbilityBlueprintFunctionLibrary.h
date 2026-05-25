// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CrysAbilityBlueprintFunctionLibrary.generated.h"

class UCombatSystemComponent;
enum class EAbilityTargetType : uint8;
/**
 * 
 */
UCLASS()
class CRYSPROJECT_API UCrysAbilityBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	/** Returns true if the TargetActor is the AbilityTargetType. Using the generic team attitude. */
	UFUNCTION(BlueprintPure, Category = "Ability|Targeting Filter")
	static bool IsAbilityTargetType(EAbilityTargetType AbilityTargetType, AActor* SourceActor, AActor* TargetActor);
	
	/** Uses the AbilityTargetInterface to get the target of an ability. */
	UFUNCTION(BlueprintPure, Category = "Ability", meta = (DefaultToSelf = "SourceActor"))
	static AActor* GetAbilityTarget(AActor* SourceActor);
	
	UFUNCTION(BlueprintPure, Category = "Ability", meta = (DefaultToSelf = Actor))
	static UCombatSystemComponent* GetCombatSystemComponent(AActor* Actor);
};

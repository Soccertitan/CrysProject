// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "MapTransitionSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FMapTransitionStartedSignature,
	const TSoftObjectPtr<UWorld>&, Map,
	const FName&, PlayerStartTag,
	const int32, TransitionDelay);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMapTransitionCancelledSignature);

/**
 * Mechanism to change the map seamlessly.
 */
UCLASS()
class CRYSPROJECT_API UMapTransitionSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "MapTransition")
	void StartMapTransition(TSoftObjectPtr<UWorld> InMap, FName PlayerStartTag, int32 TransitionDelay = 5);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "MapTransition")
	void CancelMapTransition();

	/** [Client + Server] Called whenever the MapTransition started. */
	UPROPERTY(BlueprintAssignable, DisplayName = "OnMapTransitionStarted")
	FMapTransitionStartedSignature OnMapTransitionStartedDelegate;

	/** [Client + Server] Called when a map transition is cancelled. */
	UPROPERTY(BlueprintAssignable, DisplayName = "OnMapTransitionCancelled")
	FMapTransitionCancelledSignature OnMapTransitionCancelledDelegate;

protected:
	virtual bool DoesSupportWorldType(const EWorldType::Type WorldType) const override;

private:
	/** The map to seamless transition to. */
	UPROPERTY(EditAnywhere, Category = "MapTransition")
	TSoftObjectPtr<UWorld> Map;

	/** The PlayerStart point to transition to. */
	UPROPERTY(EditAnywhere, Category = "MapTransition")
	FName PlayerStartTag;

	/** The number of seconds to wait before transitioning to the new map. */
	UPROPERTY(EditAnywhere, Category = "MapTransition")
	int32 TransitionDelay;

	UPROPERTY()
	FTimerHandle TimerHandle_GoToMap;
	UPROPERTY()
	bool bIsTransitioning = false;

	void TransitionToMap();
	void StartMapTransitionTimer();

	UFUNCTION(NetMulticast, Reliable)
	void Multi_OnMapTransitionStarted(const TSoftObjectPtr<UWorld>& InMap, const FName& InPlayerStartTag, const int32 InTransitionDelay);
	UFUNCTION(NetMulticast, Reliable)
	void Multi_OnMapTransitionCancelled();
};

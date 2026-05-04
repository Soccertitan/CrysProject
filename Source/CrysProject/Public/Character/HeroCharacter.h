// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "CrysCharacter.h"
#include "HeroCharacter.generated.h"

/**
 * Player's will use this class to control their Hero.
 */
UCLASS()
class CRYSPROJECT_API AHeroCharacter : public ACrysCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AHeroCharacter(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};

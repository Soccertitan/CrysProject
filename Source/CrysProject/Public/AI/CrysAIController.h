// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CrysAIController.generated.h"

UCLASS()
class CRYSPROJECT_API ACrysAIController : public AAIController
{
	GENERATED_BODY()

public:
	ACrysAIController();

	// IGenericTeamAgentInterface
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override;
};

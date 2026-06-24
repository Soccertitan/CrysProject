// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CrysCharacter.h"

#include "TargetPointComponent.h"
#include "AI/CrysAIController.h"
#include "Character/CrysCharacterMovementComponent.h"
#include "Character/CrysSkeletalMeshComponent.h"
#include "Net/UnrealNetwork.h"


ACrysCharacter::ACrysCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.
		SetDefaultSubobjectClass<UCrysCharacterMovementComponent>(CharacterMovementComponentName).
		SetDefaultSubobjectClass<UCrysSkeletalMeshComponent>(MeshComponentName))
{
	bReplicates = true;
	bUseControllerRotationYaw = false;
	bReplicateUsingRegisteredSubObjectList = true;
	
	AIControllerClass = ACrysAIController::StaticClass();
	
	TargetPointComponent = CreateDefaultSubobject<UTargetPointComponent>(TEXT("TargetPointComponent"));
	TargetPointComponent->SetupAttachment(RootComponent);
}

void ACrysCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, CharacterName, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, SkeletalMeshMergeParams, COND_None, REPNOTIFY_Always);
}

void ACrysCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	MergeSkeletalMeshes();
}

void ACrysCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	MergeSkeletalMeshes();
	OnRep_SkeletalMeshMergeParams();
}

void ACrysCharacter::SetCharacterName(const FText Name)
{
	if (HasAuthority())
	{
		CharacterName = Name;
		OnCharacterNameUpdatedDelegate.Broadcast(CharacterName);
		OnRep_CharacterName();
	}
}

void ACrysCharacter::OnRep_CharacterName() const
{
	OnCharacterNameUpdatedDelegate.Broadcast(CharacterName);
}

void ACrysCharacter::OnRep_SkeletalMeshMergeParams()
{
	MergeSkeletalMeshes();
}

void ACrysCharacter::MergeSkeletalMeshes()
{
	if (SkeletalMeshMergeParams.BaseMeshesToMerge.Num() > 0 ||
		SkeletalMeshMergeParams.DynamicMeshesToMerge.Num() > 0)
	{
		if (USkeletalMesh* MergedMesh = UCrimSkeletalMergingLibrary::MergeMeshes(SkeletalMeshMergeParams))
		{
			GetMesh()->SetSkeletalMesh(MergedMesh, false);
		}
	}
}



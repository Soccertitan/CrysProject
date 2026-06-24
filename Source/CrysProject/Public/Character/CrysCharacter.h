// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CrimSkeletalMergingLibrary.h"
#include "GameFramework/Character.h"
#include "CrysCharacter.generated.h"

class UTargetPointComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCrysCharacterNameSignature, FText, CharacterName);

/**
 * The base character class used in this project.
 */
UCLASS()
class CRYSPROJECT_API ACrysCharacter : public ACharacter
{
	GENERATED_BODY()

	//TODO: Create a data asset to drive character skeleton and other data. To avoid having to create a new BP for each and every different type of character. IE NPC, Enemy, Hero, that could use the same skeleton.
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TargetPoint", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTargetPointComponent> TargetPointComponent;
	
public:
	ACrysCharacter(const FObjectInitializer& ObjectInitializer);
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	
	// [Client + Server] When the character name is set.
	UPROPERTY(BlueprintAssignable, DisplayName = OnCharacterNameUpdated)
	FCrysCharacterNameSignature OnCharacterNameUpdatedDelegate;
	
	UFUNCTION(BlueprintPure, Category = "Character")
	FText GetCharacterName() const {return CharacterName;}

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Character")
	void SetCharacterName(const FText Name);

protected:
	UFUNCTION()
	void OnRep_CharacterName() const;

	UFUNCTION()
	void OnRep_SkeletalMeshMergeParams();

	void MergeSkeletalMeshes();
	
private:
	UPROPERTY(EditAnywhere, ReplicatedUsing=OnRep_CharacterName, Category = "Character")
	FText CharacterName;

	UPROPERTY(EditAnywhere, ReplicatedUsing=OnRep_SkeletalMeshMergeParams, Category = "Character")
	FCrimSkeletalMeshMergeParams SkeletalMeshMergeParams;
};

// Copyright Soccertitan 2026

#pragma once

#include "CoreMinimal.h"
#include "Components/Image.h"
#include "CrysImage.generated.h"

/**
 * Helper functions to be compatible with ViewModels.
 */
UCLASS()
class CRYSPROJECT_API UCrysImage : public UImage
{
	GENERATED_BODY()
	
public:
	// The texture to use when an invalid Texture is set.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTexture2D> DefaultTexture;
	
	virtual void SetBrushFromTexture(UTexture2D* Texture, bool bMatchSize = false) override;
	virtual void SetBrushFromSoftTexture(TSoftObjectPtr<UTexture2D> SoftTexture, bool bMatchSize = false) override;
	
	/** Calls SetBrushFromTexture internally with bMatchSize = true. */
	UFUNCTION(BlueprintCallable, Category = "Appearance")
	void SetBrushFromTextureMatchSize(UTexture2D* Texture);
	
	/** Calls SetBrushFromTexture internally with bMatchSize = false. */
	UFUNCTION(BlueprintCallable, Category = "Appearance")
	void SetBrushFromTextureNotMatchSize(UTexture2D* Texture);
	
	/** Calls SetBrushFromSoftTexture internally with bMatchSize = true. */
	UFUNCTION(BlueprintCallable, Category = "Appearance")
	void SetBrushFromSoftTextureMatchSize(TSoftObjectPtr<UTexture2D> SoftTexture);
	
	/** Calls SetBrushFromSoftTexture internally with bMatchSize = false. */
	UFUNCTION(BlueprintCallable, Category = "Appearance")
	void SetBrushFromSoftTextureNotMatchSize(TSoftObjectPtr<UTexture2D> SoftTexture);
};

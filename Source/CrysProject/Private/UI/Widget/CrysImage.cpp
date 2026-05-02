// Copyright Soccertitan 2026


#include "CrysImage.h"

void UCrysImage::SetBrushFromTexture(UTexture2D* Texture, bool bMatchSize)
{
	if (Texture)
	{
		Super::SetBrushFromTexture(Texture, bMatchSize);
	}
	else
	{
		Super::SetBrushFromTexture(DefaultTexture, bMatchSize);
	}
}

void UCrysImage::SetBrushFromSoftTexture(TSoftObjectPtr<UTexture2D> SoftTexture, bool bMatchSize)
{
	if (SoftTexture.IsNull())
	{
		SetBrushFromTexture(DefaultTexture, bMatchSize);
	}
	else
	{
		Super::SetBrushFromSoftTexture(SoftTexture, bMatchSize);	
	}
}

void UCrysImage::SetBrushFromTextureMatchSize(UTexture2D* Texture)
{
	SetBrushFromTexture(Texture, true);
}

void UCrysImage::SetBrushFromTextureNotMatchSize(UTexture2D* Texture)
{
	SetBrushFromTexture(Texture, false);
}

void UCrysImage::SetBrushFromSoftTextureMatchSize(TSoftObjectPtr<UTexture2D> SoftTexture)
{
	SetBrushFromSoftTexture(SoftTexture, true);
}

void UCrysImage::SetBrushFromSoftTextureNotMatchSize(TSoftObjectPtr<UTexture2D> SoftTexture)
{
	SetBrushFromSoftTexture(SoftTexture, false);
}

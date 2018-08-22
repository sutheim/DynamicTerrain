// Fill out your copyright notice in the Description page of Project Settings.

#include "RTProceduralMeshComponent.h"


FColor URTProceduralMeshComponent::UpdateBuffer(int32 index)
{
	ColorBuffer.Reset();

	if (RenderTarget != NULL)
	{

		FTextureRenderTarget2DResource* textureResource = (FTextureRenderTarget2DResource*)RenderTarget->Resource;
		if (textureResource->ReadPixels(ColorBuffer))
		{
			return ColorBuffer[index];
		}
	}

	return FColor();
	//FString colorAsString = ColorBuffer[0].ToString();
	//FString length = ColorBuffer[0].ToString();

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, length);
}
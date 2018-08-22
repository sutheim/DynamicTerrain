// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <EngineGlobals.h>
#include <Runtime/Engine/Classes/Engine/Engine.h>
#include "Engine/TextureRenderTarget2D.h"
#include "ProceduralMeshComponent.h"
#include "RTProceduralMeshComponent.generated.h"

/**
 * 
 */
UCLASS(hidecategories = (Object, LOD), meta = (BlueprintSpawnableComponent), ClassGroup = Rendering)
class DYNAMICTERRAIN_API URTProceduralMeshComponent : public UProceduralMeshComponent
{
	GENERATED_BODY()
	
	UPROPERTY(Category = HeightMap, EditAnywhere)
	UTextureRenderTarget2D* RenderTarget;

	UFUNCTION(BlueprintCallable, Category = "HeightMap|Update")
	FColor UpdateBuffer(int32 index);

private:

	TArray<FColor> ColorBuffer;
	
};

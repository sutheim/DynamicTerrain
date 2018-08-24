// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
#include "DynamicTerrainComponent.generated.h"

/**
 * 
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class DYNAMICTERRAIN_API UDynamicTerrainComponent : public UProceduralMeshComponent
{
	GENERATED_BODY()
	
public:
	UDynamicTerrainComponent();

private:
	TArray<FVector> terrainVertices;
	TArray<int32> terrainTriangles;
	TArray<FVector> terrainNormals;
	TArray<FVector2D> terrainUV0;
	TArray<FColor> terrainVertexColors;
	TArray<FProcMeshTangent> terrainTangents;



	UPROPERTY(EditAnywhere)
	int terrainResolution = 128;
	
	UPROPERTY(EditAnywhere)
	int terrainWidth = 1024;
	
	UFUNCTION(BlueprintCallable)
	void initializeTerrainArray();
};

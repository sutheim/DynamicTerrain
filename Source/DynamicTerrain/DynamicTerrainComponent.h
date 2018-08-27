// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimplexNoise.h"
#include "UnrealMathUtility.h"
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

	UFUNCTION(BlueprintCallable)
		void InitializeTerrainArray();

	UFUNCTION(BlueprintCallable)
		void InitializeNormals();

	UFUNCTION(BlueprintCallable)
		void ModifyTerrain(FVector location, float radius, float intensity);

	UPROPERTY(EditAnywhere)
		float terrainHeight = 64;

	UPROPERTY(EditAnywhere)
		int32 terrainResolution = 128;

	UPROPERTY(EditAnywhere)
		float terrainWidth = 4096;

	UPROPERTY(EditAnywhere)
		float terrainNoiseScale = 0.0008;

private:
	TArray<FVector> terrainVertices;
	TArray<FVector>* terrainVerticesPtr = &terrainVertices;
	TArray<int32> terrainTriangles;
	TArray<FVector> terrainNormals;
	TArray<FVector2D> terrainUV0;
	TArray<FColor> terrainVertexColors;
	TArray<FProcMeshTangent> terrainTangents;

	TArray<int32> GetIndicesInRadius(int32 xIndex, int32 yIndex, int32 radius);

	float GetDistanceToIndex(int32 center, int32 index);

	FVector2D GetXYfromIndex(int32 index);

	int32 GetIndexFromXY(FVector2D xyIndex);

	FVector GetNormalFromIndex(int32 index);
};

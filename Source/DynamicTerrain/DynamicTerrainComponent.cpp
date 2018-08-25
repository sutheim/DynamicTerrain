// Fill out your copyright notice in the Description page of Project Settings.

#include "DynamicTerrainComponent.h"


UDynamicTerrainComponent::UDynamicTerrainComponent()
{


}

void UDynamicTerrainComponent::initializeTerrainArray()
{
	//Initialize vertices

	UE_LOG(LogTemp, Log, TEXT("Initializing mesh with resolution: %d"), terrainResolution);

	for (int yIndex = 0; yIndex < terrainResolution; yIndex = yIndex + 1) {
		for (int xIndex = 0; xIndex < terrainResolution; xIndex = xIndex + 1) {

			int index = terrainResolution * yIndex + xIndex;

			float distanceBetweenPoints = terrainWidth / terrainResolution;

			float xCoord = xIndex * distanceBetweenPoints;
			float yCoord = yIndex * distanceBetweenPoints;


			terrainVertices.Add(FVector(xCoord, yCoord, SimplexNoise::sNoise(xCoord * terrainNoiseScale, yCoord * terrainNoiseScale) * terrainHeight));

			terrainUV0.Add(FVector2D(xIndex / float(terrainResolution), yIndex / float(terrainResolution)));

			if (xIndex != terrainResolution-1)
			{
				int32 point1 = index;
				int32 point2 = point1 + 1;
				int32 point3 = point1 + terrainResolution;
				int32 point4 = point2 + terrainResolution;

				terrainTriangles.Add(point1);
				terrainTriangles.Add(point3);
				terrainTriangles.Add(point2);

				terrainTriangles.Add(point3);
				terrainTriangles.Add(point4);
				terrainTriangles.Add(point2);
				

			}

		}
	}


	Super::CreateMeshSection(0,terrainVertices,terrainTriangles,terrainNormals,terrainUV0,terrainVertexColors,terrainTangents,true);

}

void UDynamicTerrainComponent::ModifyTerrain(FVector location, float radius, float intensity)
{
	int xIndex = FMath::RoundHalfFromZero(location[0] / terrainWidth);
	int yIndex = FMath::RoundHalfFromZero(location[1] / terrainWidth);

	int32 index = terrainResolution * yIndex + xIndex;

	UE_LOG(LogTemp, Log, TEXT("terrainVertices array length is: %d"), terrainVertices.Num());

	//terrainVertices.Insert(FVector(terrainVertices[index].X, terrainVertices[index].Y, terrainVertices[index].Z - 25.0f), index);

	//Super::CreateMeshSection(0, terrainVertices, terrainTriangles, terrainNormals, terrainUV0, terrainVertexColors, terrainTangents, true);
}

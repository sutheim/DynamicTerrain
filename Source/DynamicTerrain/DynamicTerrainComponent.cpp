// Fill out your copyright notice in the Description page of Project Settings.

#include "DynamicTerrainComponent.h"


UDynamicTerrainComponent::UDynamicTerrainComponent()
{
	//InitializeTerrainArray();
}

void UDynamicTerrainComponent::InitializeTerrainArray()
{
	//Initialize landscape mesh with some noise

	UE_LOG(LogTemp, Log, TEXT("Initializing mesh with resolution: %d, terrainVertices stored at: %d"), terrainResolution, terrainVerticesPtr);


	//check this badboi
	check(terrainVerticesPtr);

	//loop through the rows and collumns according to the resolution ( will always be square )
	for (int yIndex = 0; yIndex < terrainResolution; yIndex = yIndex + 1) {
		for (int xIndex = 0; xIndex < terrainResolution; xIndex = xIndex + 1) {

			int index = terrainResolution * yIndex + xIndex;

			float distanceBetweenPoints = terrainWidth / terrainResolution;

			float xCoord = xIndex * distanceBetweenPoints;
			float yCoord = yIndex * distanceBetweenPoints;



			//Add the vertices with some simple noise added to the height
			terrainVertices.Add(FVector(xCoord, yCoord, SimplexNoise::sNoise(xCoord * terrainNoiseScale, yCoord * terrainNoiseScale) * terrainHeight));

			//Add UVs that will align with the size of the terrain (should possibly add a switch for constant size tiling uvs?)
			terrainUV0.Add(FVector2D(xIndex / float(terrainResolution), yIndex / float(terrainResolution)));

			//Detect whether we are on an edge that does not connect to anything
			if (xIndex != terrainResolution-1 && yIndex != terrainResolution-1)
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
	UE_LOG(LogTemp, Log, TEXT("terrainVertices array length is: %d after creation, with a memory location of: %d"), terrainVertices.Num(), &terrainVertices);

}

void UDynamicTerrainComponent::ModifyTerrain(FVector location, float radius, float intensity)
{
	int xIndex = FMath::RoundHalfFromZero((location[0] / terrainWidth) * terrainResolution);
	int yIndex = FMath::RoundHalfFromZero((location[1] / terrainWidth) * terrainResolution);

	int32 index = terrainResolution * yIndex + xIndex;
	
	UE_LOG(LogTemp, Log, TEXT("Index of hit: %d. Based on xIndex: %d, and yIndex %d"), index,xIndex,yIndex);

	UE_LOG(LogTemp, Log, TEXT("terrainVertices array length is: %d, with a memory location of: %d"), terrainVertices.Num(), &terrainVertices);

	FVector newVector = FVector(terrainVertices[index].X, terrainVertices[index].Y, terrainVertices[index].Z - 25.0f);

	terrainVertices[index] = newVector;

	Super::CreateMeshSection(0, terrainVertices, terrainTriangles, terrainNormals, terrainUV0, terrainVertexColors, terrainTangents, true);
}

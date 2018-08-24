// Fill out your copyright notice in the Description page of Project Settings.

#include "DynamicTerrainComponent.h"


UDynamicTerrainComponent::UDynamicTerrainComponent()
{


}

void UDynamicTerrainComponent::initializeTerrainArray()
{
	//Initialize vertices
	for (int yIndex = 0; yIndex < terrainResolution; yIndex = yIndex + 1) {
		for (int xIndex = 0; xIndex < terrainResolution; xIndex = xIndex + 1) {

			int index = xIndex * yIndex + xIndex;

			float distanceBetweenPoints = terrainWidth / terrainResolution;

			float xCoord = xIndex * distanceBetweenPoints;
			float yCoord = yIndex * distanceBetweenPoints;

			terrainVertices.Add(FVector(xCoord, yCoord, 0.0f));

			if (yIndex != terrainResolution-1 && xIndex != terrainResolution-1)
			{
				int32 point1 = xIndex * yIndex + xIndex;
				int32 point2 = point1 + 1;
				int32 point3 = point1 + terrainResolution;
				int32 point4 = point2 + terrainResolution;

				terrainTriangles.Add(point1);
				terrainTriangles.Add(point3);
				terrainTriangles.Add(point2);

				terrainTriangles.Add(point4);
				terrainTriangles.Add(point2);
				terrainTriangles.Add(point3);


				//UE_LOG(LogTemp, Log, TEXT("X index is: %d and points 1-4 are: %d,%d,%d,%d "), xIndex, point1,point2,point3,point4);

			}
		}
	}


	Super::CreateMeshSection(0,terrainVertices,terrainTriangles,terrainNormals,terrainUV0,terrainVertexColors,terrainTangents,true);

}

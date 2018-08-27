// Fill out your copyright notice in the Description page of Project Settings.

#include "DynamicTerrainComponent.h"


UDynamicTerrainComponent::UDynamicTerrainComponent()
{
	bUseAsyncCooking = true;
	terrainVertices.Reserve(terrainResolution*terrainResolution);
	terrainUV0.Reserve(terrainResolution*terrainResolution);
	terrainNormals.Reserve(terrainResolution*terrainResolution);
}

//Initializes array with vertices, triangles, UVs, normals and some noise in the heightmap
void UDynamicTerrainComponent::InitializeTerrainArray()
{
	//Initialize landscape mesh with some noise

	UE_LOG(LogTemp, Log, TEXT("Initializing mesh with resolution: %d"), terrainResolution);


	//check this badboi
	check(terrainVerticesPtr);

	//loop through the rows and collumns according to the resolution ( will always be square )
	for (int yIndex = 0; yIndex < terrainResolution; yIndex++)
	{
		for (int xIndex = 0; xIndex < terrainResolution; xIndex++)
		{
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
	UE_LOG(LogTemp, Log, TEXT("terrainVertices array length is: %d after creation"), terrainVertices.Num());

}

void UDynamicTerrainComponent::InitializeNormals()
{
	for (int32 i = 0; i < terrainVertices.Num(); i++)
	{
		terrainNormals.Add(GetNormalFromIndex(i));
	}

	Super::CreateMeshSection(0, terrainVertices, terrainTriangles, terrainNormals, terrainUV0, terrainVertexColors, terrainTangents, true);
	UE_LOG(LogTemp, Log, TEXT("NORMAL CALCULATION COMPLETE"));
}


//Used to apply an explosive modification to the terrain
void UDynamicTerrainComponent::ModifyTerrain(FVector location, float radius, float intensity)
{
	int32 xIndex = FMath::RoundHalfFromZero((location[0] / terrainWidth) * terrainResolution);
	int32 yIndex = FMath::RoundHalfFromZero((location[1] / terrainWidth) * terrainResolution);

	int32 locationIndex = GetIndexFromXY(FVector2D(xIndex, yIndex));
	
	UE_LOG(LogTemp, Log, TEXT("Index of hit: %d. Based on xIndex: %d, and yIndex %d"), locationIndex ,xIndex,yIndex);

	//This bits fucked
	
	TArray<int32> affectedIndices = GetIndicesInRadius(xIndex, yIndex, radius);

	for (int i = 0; i < affectedIndices.Num(); i++)
	{
		int32 affectedIndex = affectedIndices[i];
		float falloff = FMath::Clamp((GetDistanceToIndex(locationIndex, affectedIndex) * -1 + radius) / radius, 0.0f, 1.0f);

		FVector newVector = FVector(terrainVertices[affectedIndex].X, terrainVertices[affectedIndex].Y, terrainVertices[affectedIndex].Z - intensity * FMath::Pow(FMath::SmoothStep(0,1,falloff),0.9));
		
		terrainVertices[affectedIndex] = newVector;
	}


	Super::CreateMeshSection(0, terrainVertices, terrainTriangles, terrainNormals, terrainUV0, terrainVertexColors, terrainTangents, true);
}


// Gets a square of points around the hit location based on the radius, to avoid editing complete arrays of data
TArray<int32> UDynamicTerrainComponent::GetIndicesInRadius(int32 xIndex,int32 yIndex, int32 radius)
{
	TArray<int32> indicesInRadius;
	int32 xIndexAdjusted = xIndex - radius;
	int32 yIndexAdjusted = yIndex - radius;

	int32 outX;
	int32 outY;


	for (int i = 0; i <= radius*2; i++)
	{
		for (int j = 0; j <= radius*2; j++)
		{
			outX = xIndexAdjusted + i;
			outY = yIndexAdjusted + j;

			//Test to check index is not out of bounds
			if (outX >= 0 && outY >= 0 && outX < terrainResolution && outY < terrainResolution)
			{
				indicesInRadius.Add(GetIndexFromXY(FVector2D(outX, outY)));
			}
		}
	}

	return indicesInRadius;
}

//Calculates vertex normal for any given index in the array
FVector UDynamicTerrainComponent::GetNormalFromIndex(int32 index)
{
	FVector2D xyIndex = GetXYfromIndex(index);
	FVector horizontal;
	FVector vertical;

	//If the point is not on an edge
	if (xyIndex.X > 0 && xyIndex.Y > 0 && xyIndex.X < terrainResolution-1 && xyIndex.Y < terrainResolution-1)
	{
		FVector horizontal = terrainVertices[index-1] - terrainVertices[index+1];

		FVector vertical = terrainVertices[index + terrainResolution] - terrainVertices[index - terrainResolution];

		
	}
	else
	{
		return FVector(0, 0, 1);

		//If the point is either on the leftmost or rightmost edge
		if (xyIndex.X == terrainResolution-1)
		{
			FVector horizontal = FVector(0, terrainVertices[index].Z - terrainVertices[index - 1].Z, 1);
		}
		else if(xyIndex.X == 0)
		{
			FVector horizontal = FVector(0, terrainVertices[index + 1].Z - terrainVertices[index].Z, 1);
		}
		
		//If the point is either on the top or bottom edge
		if (xyIndex.Y == terrainResolution-1)
		{
			FVector vertical = FVector(0, terrainVertices[index].Z - terrainVertices[index - terrainResolution].Z, 1);
		}
		else
		{
			FVector vertical = FVector(0, terrainVertices[index+1].Z - terrainVertices[index].Z, 1);
		}
	}

	FVector normal = FVector(vertical.Y*horizontal.Z - vertical.Z - horizontal.Y, vertical.Z*horizontal.X - vertical.X*horizontal.Z, vertical.X*horizontal.Y - vertical.Y - horizontal.X).GetSafeNormal();
	if (index % 1000 == 0)
	{
		UE_LOG(LogTemp, Log, TEXT("NORMAL CALCULATION--- current normal is: %f, %f, %f"), normal.X, normal.Y, normal.Z);
		UE_LOG(LogTemp, Log, TEXT("NORMAL CALCULATION--- current horizontal: %f, %f, %f"), horizontal.X, horizontal.Y, horizontal.Z);
		UE_LOG(LogTemp, Log, TEXT("NORMAL CALCULATION--- current vertical: %f, %f, %f"), vertical.X, vertical.Y, vertical.Z);
	}
	
	return normal;
	//return FVector(0,0,1);
}

//Different helper functions for navigating terrain arrays

float UDynamicTerrainComponent::GetDistanceToIndex(int32 center, int32 index)
{
	return FVector2D::Distance(GetXYfromIndex(center), GetXYfromIndex(index));
}

FVector2D UDynamicTerrainComponent::GetXYfromIndex(int32 index)
{
	return FVector2D(FMath::Fmod(index, terrainResolution), int32(index / terrainResolution));
}

int32 UDynamicTerrainComponent::GetIndexFromXY(FVector2D xyIndex)
{
	return terrainResolution * xyIndex.Y + xyIndex.X;
}

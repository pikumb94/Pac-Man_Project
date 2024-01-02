
#pragma once
#include "CoreMinimal.h"

namespace GridConstants
{
	const float GridSize = 100.f;

	// The reference system is Y-down
	const TArray<FVector, TFixedAllocator<4>> GridVersorsArray{
		-FVector::RightVector,		//Up, North
		-FVector::ForwardVector,	//Left, West
		FVector::RightVector,		//Down, South
		FVector::ForwardVector,		//Right, East
	};

}

// Snaps a generic vector in a volumetric grid of CustomGridSize. If the CustomGridSize is not specified or is invalid
// the GridSize constant is used
inline FVector VectorGridSnap(const FVector& Point, float CustomGridSize = 0.f)
{
	FVector SnappedVector;
	if (CustomGridSize > 0.f) {
		SnappedVector.X = FMath::GridSnap(Point.X, CustomGridSize);
		SnappedVector.Y = FMath::GridSnap(Point.Y, CustomGridSize);
		SnappedVector.Z = FMath::GridSnap(Point.Z, CustomGridSize);
	}
	else {
		SnappedVector.X = FMath::GridSnap(Point.X, GridConstants::GridSize);
		SnappedVector.Y = FMath::GridSnap(Point.Y, GridConstants::GridSize);
		SnappedVector.Z = FMath::GridSnap(Point.Z, GridConstants::GridSize);
	}
	
	return SnappedVector;
	
}
/*
//returns the index of a given direction in the GridVersorArray
inline int directionArrayIndex(FVector dir) {

	if (dir == -FVector::RightVector)
		return 0;

	if (dir == -FVector::ForwardVector)
		return 1;

	if (dir == FVector::RightVector)
		return 2;

	if (dir == FVector::ForwardVector)
		return 3;

	return -1;
}*/
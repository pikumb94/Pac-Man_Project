// Fill out your copyright notice in the Description page of Project Settings.


#include "PacManEnemyAIController.h"

#include "GridUtilities.h"
#include "GridPawn.h"


#pragma optimize("", off)

void APacManEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	ChangeEnemyState(EEnemyState::Scatter);

	ControlledGridPawn = Cast<AGridPawn>(GetPawn());
	
	CurrentCell = VectorGridSnap(ControlledGridPawn->GetActorLocation());
	FVector NextDirection = DecideNextDirection();
	ControlledGridPawn->ForceDirection(NextDirection);

	NextCell = CurrentCell + NextDirection * GridConstants::GridSize;
}

void APacManEnemyAIController::ChangeEnemyState(EEnemyState NewState)
{
	//(1500, -1500, 0)
	switch (NewState)
	{
		case EEnemyState::Scatter:
			if (EnemyType == EEnemyType::Blinky)
				TargetCell = FVector(1500, -1500, 0);
			if (EnemyType == EEnemyType::Pinky)
				TargetCell = FVector(-1500, -1500, 0);
			if (EnemyType == EEnemyType::Inky)
				TargetCell = FVector(1500, 1500, 0);
			if (EnemyType == EEnemyType::Clyde)
				TargetCell = FVector(-1500, 1500, 0);

			break;
		case EEnemyState::Chase:

			break;
		case EEnemyState::Frightened:

			break;
		default:
			//In any other case return to the ghosthouse
			TargetCell = FVector::ZeroVector;
			break;
	}

}

FVector APacManEnemyAIController::DecideNextDirection()
{
	//Get Available directions: use linetrace to find free cells)
	TArray<bool, TFixedAllocator<4>> HitVector;
	FVector TraceStart = CurrentCell;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetPawn());

	for (auto& versor : GridConstants::GridVersorsArray) {
		FHitResult Hit;

		FVector TraceEnd = CurrentCell + versor * GridConstants::GridSize;
		GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_WorldStatic, QueryParams);
		DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Magenta);

		HitVector.Add(Hit.bBlockingHit);
	}

	//Exclude opposite direction
	int indexToExclude = -1;
	if (GridConstants::GridVersorsArray.Find(-ControlledGridPawn->GetMovingDirection(), indexToExclude))
		HitVector[indexToExclude] = true;

	//Find next direction among free cell by choosing the closest one
	float minDstSqrd = TNumericLimits<float>::Max();
	int minIdx = -1;

	for (int i = 0; i < HitVector.Num(); i++)
	{
		if (HitVector[i])
			continue;

		FVector TmpNextCell = CurrentCell + GridConstants::GridVersorsArray[i] * GridConstants::GridSize;

		float dstSqrd = FMath::Pow(TmpNextCell.X - TargetCell.X, 2) + FMath::Pow(TmpNextCell.Y - TargetCell.Y, 2);

		if (dstSqrd < minDstSqrd) {
			minIdx = i;
			minDstSqrd = dstSqrd;
		}
	}


	return GridConstants::GridVersorsArray[minIdx];
}

void APacManEnemyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CurrentCell = VectorGridSnap(ControlledGridPawn->GetActorLocation());
	FVector CurrentLocation = ControlledGridPawn->GetActorLocation();

	if ((CurrentLocation-NextCell).Size() <12.5f || (CurrentCell - NextCell).Size() > GridConstants::GridSize) {

		FVector NextDirection = DecideNextDirection();
		ControlledGridPawn->ForceDirection(NextDirection);

		NextCell = CurrentCell + NextDirection * GridConstants::GridSize;

	}

	
	DrawDebugLine(GetWorld(), ControlledGridPawn->GetActorLocation(), ControlledGridPawn->GetActorLocation()+ControlledGridPawn->GetMovingDirection()*100, FColor::Green);
}
#pragma optimize("", on)

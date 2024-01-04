// Fill out your copyright notice in the Description page of Project Settings.


#include "PacManEnemyAIController.h"

#include "GridUtilities.h"
#include "PacManGameMode.h"
#include "EnemyGridPawn.h"
#include "PacManGameInstance.h"

#pragma optimize("", off)

APacManEnemyAIController::APacManEnemyAIController()
{

}


void APacManEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	TObjectPtr<APacManGameMode> GM = Cast<APacManGameMode>(GetWorld()->GetAuthGameMode());

	if (GM) {
		GM->OnFrightenedChanged.AddDynamic(this, &APacManEnemyAIController::OnFrightenedHandler);
	}

}

void APacManEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ControlledGridPawn = Cast<AEnemyGridPawn>(InPawn);

	ChangeEnemyState(EEnemyState::Scatter);

	CurrentCell = VectorGridSnap(ControlledGridPawn->GetActorLocation());
	FVector NextDirection = DecideNextDirection();
	ControlledGridPawn->ForceDirection(NextDirection);

	NextCell = CurrentCell + NextDirection * GridConstants::GridSize;
}

void APacManEnemyAIController::ChangeEnemyState(EEnemyState NewState)
{
	
	auto EnemyData = Cast<APacManGameMode>(GetWorld()->GetAuthGameMode())->GetEnemiesData();
	

	switch (NewState)
	{
		case EEnemyState::Scatter:
			TargetCell = EnemyData->GetEnemyScatterCell(EnemyType);
			ControlledGridPawn->FrightenedBlinkMaterial(false);

			break;

		case EEnemyState::Chase:
			ControlledGridPawn->FrightenedBlinkMaterial(false);

			break;

		case EEnemyState::Frightened:
			ControlledGridPawn->FrightenedBlinkMaterial(true);

			break;

		default:
			//In any other case return to the ghosthouse
			ControlledGridPawn->FrightenedBlinkMaterial(false);
			TargetCell = FVector::ZeroVector;
			break;
	}

	State = NewState;
}

void APacManEnemyAIController::OnFrightenedHandler(bool NewFrightenedValue)
{
	if (NewFrightenedValue) {
		ChangeEnemyState(EEnemyState::Frightened);
	}
	else
		ChangeEnemyState(EEnemyState::Scatter); //	TODO we should set back the state before frightened mode

}

void APacManEnemyAIController::PawnOverlappedPlayerHandler()
{
	TObjectPtr<APacManGameMode> GM = Cast<APacManGameMode>(GetWorld()->GetAuthGameMode());

	if (GM) {

		if (State == EEnemyState::Frightened) {

			GetWorldTimerManager().SetTimerForNextTick([GM,this]() {
				ControlledGridPawn->SetActorLocation(GM->GetEnemiesData()->GetEnemyInitialCell(EnemyType));

			});

			//TODO: GIVE INCREASING SCORE TO PLAYER=> for now a fixed amount
			TObjectPtr<UPacManGameInstance> GI = GetWorld()->GetGameInstance<UPacManGameInstance>();
			if (GI)
			{
				GI->AddScore(1000);
			}
			//
		}
		else {
			GM->ReloadLevel(true);

		}
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
		GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_WorldDynamic, QueryParams);
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


	return (minIdx>-1? GridConstants::GridVersorsArray[minIdx] : FVector::ZeroVector);
}

void APacManEnemyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CurrentCell = VectorGridSnap(ControlledGridPawn->GetActorLocation());
	FVector CurrentLocation = ControlledGridPawn->GetActorLocation();

	if ((CurrentLocation-NextCell).Size() <12.5f || (CurrentCell - NextCell).Size() > GridConstants::GridSize || ControlledGridPawn->GetVelocity().SizeSquared2D()<=0.f) {

		FVector NextDirection = DecideNextDirection();
		ControlledGridPawn->ForceDirection(NextDirection);

		NextCell = CurrentCell + NextDirection * GridConstants::GridSize;

	}

	
	DrawDebugLine(GetWorld(), ControlledGridPawn->GetActorLocation(), ControlledGridPawn->GetActorLocation()+ControlledGridPawn->GetMovingDirection()*100, FColor::Green);
}
#pragma optimize("", on)

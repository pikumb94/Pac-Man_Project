// Fill out your copyright notice in the Description page of Project Settings.


#include "PacManEnemyAIController.h"

#include "GridUtilities.h"
#include "Framework/PacManGameMode.h"
#include "EnemyGridPawn.h"
#include "Framework/PacManGameInstance.h"
#include "Kismet/GameplayStatics.h"


APacManEnemyAIController::APacManEnemyAIController()
{
	PrimaryActorTick.bCanEverTick = false;
	bAsyncPhysicsTickEnabled = true;
}


void APacManEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
}

void APacManEnemyAIController::OnUnPossess()
{
	ControlledGridPawn = nullptr;
}

void APacManEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (TObjectPtr<APacManGameMode> GM = Cast<APacManGameMode>(GetWorld()->GetAuthGameMode())) {
		EnemyInfo = GM->GetEnemiesData()->GetEnemyInfoPtr(EnemyType);
		GM->OnChangeState.AddDynamic(this, &APacManEnemyAIController::ChangeEnemyState);
	}

	ControlledGridPawn = Cast<AEnemyGridPawn>(InPawn);

	//Invalid State: to force transition state updates
	State = StaticCast<EEnemyState>(-1);
	ChangeEnemyState(EEnemyState::Scatter);
	
	CurrentCell = VectorGridSnap(ControlledGridPawn->GetActorLocation());
	FVector NextDirection = DecideNextDirection();
	ControlledGridPawn->ForceDirection(NextDirection);

	NextCell = CurrentCell + NextDirection * GridConstants::GridSize;
	
}

void APacManEnemyAIController::ChangeEnemyState(EEnemyState NewState)
{


	//If we are not in Idle, the usual logic applies so set the state and variables accordingly
	if (State != EEnemyState::Idle) {
		switch (NewState)
		{
		case EEnemyState::Scatter:
			TargetCell = EnemyInfo->ScatterCell;
			ControlledGridPawn->SetBlinkEffectMaterial(false);
			ControlledGridPawn->SetOpacityMaterial(1.f);
			ControlledGridPawn->ResetGridVelocity();

			break;

		case EEnemyState::Chase:
			ControlledGridPawn->SetBlinkEffectMaterial(false);
			ControlledGridPawn->SetOpacityMaterial(1.f);
			ControlledGridPawn->ResetGridVelocity();

			//Force Inverse Direction and an invalid next cell to force correct NextCell computation
			ControlledGridPawn->SetActorLocation(VectorGridSnap(ControlledGridPawn->GetActorLocation()));
			ControlledGridPawn->ForceDirection(-ControlledGridPawn->GetMovingDirection());
			NextCell = FVector::ZeroVector;
			//
			break;

		case EEnemyState::Frightened:
			ControlledGridPawn->SetBlinkEffectMaterial(true);
			ControlledGridPawn->SetOpacityMaterial(1.f);
			//Apply velocity malus only when entering the first time in the frightened mode
			if (State != EEnemyState::Frightened)
				ControlledGridPawn->SetToAlteredVelocity();

			//Force Inverse Direction and an invalid next cell to force correct NextCell computation
			ControlledGridPawn->SetActorLocation(VectorGridSnap(ControlledGridPawn->GetActorLocation()));
			ControlledGridPawn->ForceDirection(-ControlledGridPawn->GetMovingDirection());
			NextCell = FVector::ZeroVector;
			//
			break;

		default:
			//In any other case is Idle so return to the ghosthouse
			ControlledGridPawn->SetBlinkEffectMaterial(false);
			ControlledGridPawn->SetOpacityMaterial(0.f);
			TargetCell = EnemyInfo->InitialCell;
			ControlledGridPawn->ResetGridVelocity();
			ControlledGridPawn->SetGridVelocity(ControlledGridPawn->GetGridVelocity() * 2);
			break;

		}

		//Lastly update the state
		State = NewState;

	}

}


void APacManEnemyAIController::PawnOverlappedPlayerHandler()
{
	TObjectPtr<APacManGameMode> GM = Cast<APacManGameMode>(GetWorld()->GetAuthGameMode());

	if (GM) {

		if (State == EEnemyState::Frightened) {

			//We set the Idle state to make the enemy return to the ghost house
			ChangeEnemyState(EEnemyState::Idle);

			TObjectPtr<UPacManGameInstance> GI = GetWorld()->GetGameInstance<UPacManGameInstance>();
			if (GI)
			{
				GI->AddScore(ghostBaseValue, true);
			}
		}
		else {

			if(State != EEnemyState::Idle)
				GM->ReloadLevel(true);

		}
	}
}

FVector APacManEnemyAIController::DecideNextDirection(bool isChangingState)
{
	//Get Available directions: use linetrace to find free cells
	TArray<bool> AvailableDirections;
	FVector TraceStart = CurrentCell;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetPawn());

	for (auto& versor : GridConstants::GridVersorsArray) {
		FHitResult Hit;

		FVector TraceEnd = CurrentCell + versor * GridConstants::GridSize;
		GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_WorldDynamic, QueryParams);
		//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Magenta);

		//We negate the logic of hit 
		AvailableDirections.Add(!Hit.bBlockingHit);
	}

	if (!isChangingState) 
	{
		//Exclude opposite direction
		int indexToExclude = -1;
		if (GridConstants::GridVersorsArray.Find(-ControlledGridPawn->GetMovingDirection(), indexToExclude))
			AvailableDirections[indexToExclude] = false;
	}

	return ApplyEnemyTypeDecision(AvailableDirections);
}

FVector APacManEnemyAIController::ApplyEnemyTypeDecision(const TArray<bool>& AvailableDirectionsArray)
{
	FVector DecidedDirection;
	switch (State)
	{
		case EEnemyState::Scatter:
			DecidedDirection = ClosestToTargetCellPolicy(AvailableDirectionsArray);

			break;

		case EEnemyState::Chase:

			UpdateChaseTargetCell();
			DecidedDirection = ClosestToTargetCellPolicy(AvailableDirectionsArray);

			break;

		case EEnemyState::Frightened:
			DecidedDirection = RandomChoicePolicy(AvailableDirectionsArray);

			break;

		default:
			//Idle or any other case
			DecidedDirection = ClosestToTargetCellPolicy(AvailableDirectionsArray);

			break;
	}

	return DecidedDirection;
}

void APacManEnemyAIController::UpdateChaseTargetCell()
{

	TObjectPtr<AGridPawn> PlayerPawn = Cast<AGridPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	switch (EnemyType)
	{
	case EEnemyType::Pinky:	//Speedy - Pink
	
		TargetCell = VectorGridSnap(PlayerPawn->GetActorLocation() + PlayerPawn->GetMovingDirection() * 4 * GridConstants::GridSize);
		break;


	case EEnemyType::Inky:	//Bashful - Cyan
		{
			FVector IntermediateCell = VectorGridSnap(PlayerPawn->GetActorLocation() + PlayerPawn->GetMovingDirection() * 2 * GridConstants::GridSize);

			TArray<AActor*> outEnemyPCs;
			//TODO: find a more performant way to get Blinky
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), APacManEnemyAIController::StaticClass(), outEnemyPCs);

			AActor** pBlinkyPC = outEnemyPCs.FindByPredicate([](const AActor* Element) {
				return Cast<APacManEnemyAIController>(Element)->EnemyType == EEnemyType::Blinky;
			});

			APacManEnemyAIController* BlinkyPC = Cast<APacManEnemyAIController>(*pBlinkyPC);
			FVector BlinkyPosition = BlinkyPC->ControlledGridPawn->GetActorLocation();

			FVector DistToRot = BlinkyPosition - IntermediateCell;
			DistToRot.RotateAngleAxis(180.0, FVector::UpVector);

			TargetCell = VectorGridSnap(IntermediateCell + DistToRot);
		}
		break;


	case EEnemyType::Clyde:	//Pokey - Orange:
		{
			double ToPlayerDistance = (PlayerPawn->GetActorLocation() - ControlledGridPawn->GetActorLocation()).Size();
			if (ToPlayerDistance < 8 * GridConstants::GridSize)
				TargetCell = EnemyInfo->ScatterCell;
			else
				TargetCell = VectorGridSnap(PlayerPawn->GetActorLocation());
		}
		break;


	default:
			//Blinky: Shadow - Red or any other case
		TargetCell = VectorGridSnap(PlayerPawn->GetActorLocation());

		break;
	}

}

FVector APacManEnemyAIController::RandomChoicePolicy(const TArray<bool>& AvailableDirectionsArray)
{
	auto Directions = GridConstants::GridVersorsArray;
	for (size_t i = 0; i < AvailableDirectionsArray.Num(); i++)
	{
		if (!AvailableDirectionsArray[i]) {
			Directions.Remove(GridConstants::GridVersorsArray[i]);
		}
	}

	int randIdx = FMath::RandRange(0, Directions.Num()-1);

	return Directions[randIdx];
}


FVector APacManEnemyAIController::ClosestToTargetCellPolicy(const TArray<bool>& AvailableDirectionsArray)
{

	//Find next direction among free cell by choosing the closest one
	float minDstSqrd = TNumericLimits<float>::Max();
	int minIdx = -1;

	for (int i = 0; i < AvailableDirectionsArray.Num(); i++)
	{
		if (!AvailableDirectionsArray[i])
			continue;

		FVector TmpNextCell = CurrentCell + GridConstants::GridVersorsArray[i] * GridConstants::GridSize;

		float dstSqrd = FMath::Pow(TmpNextCell.X - TargetCell.X, 2) + FMath::Pow(TmpNextCell.Y - TargetCell.Y, 2);

		if (dstSqrd < minDstSqrd) {
			minIdx = i;
			minDstSqrd = dstSqrd;
		}
	}

	return (minIdx > -1 ? GridConstants::GridVersorsArray[minIdx] : ControlledGridPawn->GetMovingDirection());

}


void APacManEnemyAIController::AsyncPhysicsTickActor(float DeltaTime, float SimTime)
{
	Super::AsyncPhysicsTickActor(DeltaTime, SimTime);

	if (ControlledGridPawn)
	{
		CurrentCell = VectorGridSnap(ControlledGridPawn->GetActorLocation());
		FVector CurrentLocation = ControlledGridPawn->GetActorLocation();

		bool hasReachedNextCell = hasReachedTargetGridLocation(CurrentLocation, NextCell);
		bool hasSkippedNextCell = (CurrentCell - NextCell).Size() > GridConstants::GridSize;
		bool isStuck = ControlledGridPawn->GetVelocity().SizeSquared2D() <= 0.f;

		if (hasReachedNextCell ||		//The new cell is recomputed if we reach the next cell
			hasSkippedNextCell)			//Error recovery: enemy is moving far from next cell
		{
			FVector NextDirection = DecideNextDirection();

			ControlledGridPawn->ForceDirection(NextDirection);

			NextCell = CurrentCell + NextDirection * GridConstants::GridSize;

		}

		//This is the only way to exit Idle State
		if (State == EEnemyState::Idle && hasReachedTargetGridLocation(CurrentLocation, TargetCell))
		{
			State = EEnemyState::Scatter;
			ChangeEnemyState(State);
		}
	}

}
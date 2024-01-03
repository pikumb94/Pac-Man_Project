// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Controller.h"
#include "PacManEnemyAIController.generated.h"

/**
 * This controller implements the AI of PacMan's enemies trying to mimic the original games's
 * technique: reching a target destination (cell) prior a decision based on the ghost character
 */

UENUM(BlueprintType)
enum class EEnemyState : uint8 {
	Idle = 0,		//enemy logic paused
	Scatter,
	Chase,
	Frightened
};

UENUM(BlueprintType)
enum class EEnemyType : uint8 {
	Blinky = 0,		//Shadow - Red
	Pinky,			//Speedy - Pink
	Inky,			//Bashful - Cyan
	Clyde			//Pokey - Orange
};
ENUM_RANGE_BY_FIRST_AND_LAST(EEnemyType, EEnemyType::Blinky, EEnemyType::Clyde);

UCLASS(BlueprintType, Blueprintable)
class PAC_MAN_PROJECT_API APacManEnemyAIController : public AController
{
	GENERATED_BODY()
	
	// The current grid cell the enemy is
	FVector CurrentCell = FVector::ZeroVector;

	// The cell of the grid the enemy will try to reach
	FVector TargetCell = FVector::ZeroVector;

	EEnemyState State = EEnemyState::Idle;

	EEnemyType EnemyType = EEnemyType::Blinky;

	TObjectPtr<class AGridPawn> ControlledGridPawn;

	FVector DecideNextDirection();

	FVector NextCell = FVector::ZeroVector;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ChangeEnemyState(EEnemyState NewState);
	void SetEnemyType(EEnemyType NewType) { EnemyType = NewType; };
};
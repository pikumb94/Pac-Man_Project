// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PacManGameMode.generated.h"

/**
 * It manages the new level instance: sets up the game, its events and end condition
 */

UCLASS()
class PAC_MAN_PROJECT_API APacManGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
	APacManGameMode();

	int remainingScorePellets;

	void SpawnEnemies();



public:

	// Enemy Pawn Class to spawn: this will be the blueprint version of AEnemyGridPawn
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AEnemyGridPawn> EnemyPawnClass;

	virtual void StartPlay() override;

	UFUNCTION()
	void CheckLevelCompleted(int ScoreToAdd);

	//If we reload a level because of a player completion or because of life loss
	void ReloadLevel(bool bReduceLife=false);
 
};

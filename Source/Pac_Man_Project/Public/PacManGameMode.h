// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PacManGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PAC_MAN_PROJECT_API APacManGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
	APacManGameMode();

	int remainingScorePellets;

	void SpawnEnemies();



public:

	// Enemy Pawn Class to spawn
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AEnemyGridPawn> EnemyPawnClass;

	virtual void StartPlay() override;

	UFUNCTION()
	void CheckLevelCompleted(int ScoreToAdd);

	void ReloadLevel(bool bReduceLife=false);
 
};

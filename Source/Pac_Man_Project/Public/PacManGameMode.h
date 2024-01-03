// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PacManGameMode.generated.h"

//Frightened events: triggers when the player picks an energizer
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFrightenedChanged, bool, IsFrightened);

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

	TObjectPtr<class UEnemyDataAsset> EnemiesData;

	FTimerHandle FrightenedTimerHandle;
	// How many seconds lasts the frightened mode
	UPROPERTY(EditDefaultsOnly)
	float FrightenedModeDuration = 10.f;

protected:
	// Enemy Pawn Class to spawn: this will be the blueprint version of AEnemyGridPawn
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AEnemyGridPawn> EnemyPawnClass;

public:

	virtual void StartPlay() override;

	void UpdateNCheckLevelCompleted();

	//If we reload a level because of a player completion or because of life loss
	void ReloadLevel(bool bReduceLife=false);

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnFrightenedChanged OnFrightenedChanged;

	TObjectPtr<UEnemyDataAsset> GetEnemiesData() const{ return EnemiesData; };

	void TriggerFrightened();
	void AddScoreFwd(int valueToAdd);
};

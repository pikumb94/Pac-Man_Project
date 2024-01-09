// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PacManGameMode.generated.h"

//Frightened events: triggers when the player picks an energizer
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangeState, EEnemyState, NewState);

/**
 * It manages the new level instance: sets up the game, its events and end condition
 */

UCLASS()
class PAC_MAN_PROJECT_API APacManGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
	APacManGameMode();

	int remainingScorePellets;

	void SpawnEnemies(const FLevelParamsStruct& InitParams);
	UFUNCTION()
	void SpawnEnemy(EEnemyType EnemyType, const FLevelParamsStruct& InitParams);

	TObjectPtr<class UEnemyDataAsset> EnemiesData;

	FTimerHandle FrightenedTimerHandle;
	// How many seconds lasts the frightened mode
	UPROPERTY(EditDefaultsOnly)
	float FrightenedModeDuration = 10.f;

	FTimerHandle ScatterNChaseTimerHandle;
	// How many seconds lasts the Scatter mode
	UPROPERTY(EditDefaultsOnly)
	float ScatterModeDuration = 7.f;
	// How many seconds lasts the Chase mode
	UPROPERTY(EditDefaultsOnly)
	float ChaseModeDuration = 20.f;
	void FlipFlopScatterChase();

protected:
	// Enemy Pawn Class to spawn: this will be the blueprint version of AEnemyGridPawn
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AEnemyGridPawn> EnemyPawnClass;

public:
	
	virtual void StartPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void BeginPlay() override;

	void UpdateNCheckLevelCompleted();

	//If we reload a level because of a player completion or because of life loss
	void ReloadLevel(bool bReduceLife=false);

	//This event is triggered by the timers and the pellet energizer and makes the enemies change their state accordingly
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnChangeState OnChangeState;

	TObjectPtr<UEnemyDataAsset> GetEnemiesData() const{ return EnemiesData; };

	void TriggerFrightened();
	void AddScoreFwd(int valueToAdd);
};

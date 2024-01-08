// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PacManGameInstance.generated.h"

// Event for Score increment
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScoreChanged, int, NewScore);
// Event for Lives increment
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLivesChanged, int, NewLives);
// Event for Game Over (player lives equal to zero)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameOver);

//This struct is used by the datatable (in the editor) to easily set the level parameters at the level start so to set the level difficulty
USTRUCT(Blueprintable)
struct FLevelParamsStruct: public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PlayerSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EnemySpeed;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PlayerFrightenedSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EnemyFrightenedSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int FrightenedModeDuration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ScatterModeDuration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ChaseModeDuration;
};


/**
 * The persistent game state: the relative events to update and initialize the game state are defined here
 */
UCLASS()
class PAC_MAN_PROJECT_API UPacManGameInstance : public UGameInstance
{
	GENERATED_BODY()

	int64 score = 0;
	int lives = 2;
	int level = 1;

	// The maximum allowed speed for a pawn in the game: 
	// the player and enemy speed are computed as a percentage (0-1) of this value
	UPROPERTY(EditDefaultsOnly)
	float MaxGridSpeed = 940.f;

	int IncrementalValue = 0;

	//Every bonusLifeThreshold scores, the player will get one extra life
	UPROPERTY(EditDefaultsOnly)
	int bonusLifeThreshold = 10000;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDataTable> LevelProgressionTable;


public:

	UFUNCTION(BlueprintCallable)
	void AddScore(int scoreToAdd, bool isScoreIncremental=false);
	UFUNCTION(BlueprintCallable)
	int64 GetScore() { return score; };

	UFUNCTION(BlueprintCallable)
	void SetLives(int NewLives) { lives = NewLives; OnLivesChanged.Broadcast(lives);};
	UFUNCTION(BlueprintCallable)
	int GetLives() { return lives; };

	UFUNCTION(BlueprintCallable)
	void IncrementLevel() { level++; };
	UFUNCTION(BlueprintCallable)
	int GetLevel() { return level; };

	UFUNCTION(BlueprintCallable)
	void ResetGameInstance() { score = 0; lives = 2; level = 1; ResetIncrementalValue(); };

	UFUNCTION(BlueprintCallable)
	void ResetIncrementalValue() { IncrementalValue = 0; };

	FLevelParamsStruct* GetCurrentLevelParams();

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnScoreChanged OnScoreChanged;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnLivesChanged OnLivesChanged;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnGameOver OnGameOver;
};

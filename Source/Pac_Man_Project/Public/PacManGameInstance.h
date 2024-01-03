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

	//Every bonusLifeThreshold scores, the player will get one extra life
	UPROPERTY(EditDefaultsOnly)
	int bonusLifeThreshold = 10000;

public:

	UFUNCTION(BlueprintCallable)
	void AddScore(int scoreToAdd);
	UFUNCTION(BlueprintCallable)
	int64 GetScore() { return score; };

	UFUNCTION(BlueprintCallable)
	void SetLives(int NewLives) { lives = NewLives; };
	UFUNCTION(BlueprintCallable)
	int GetLives() { return lives; };

	UFUNCTION(BlueprintCallable)
	void IncrementLevel() { level++; };
	UFUNCTION(BlueprintCallable)
	int GetLevel() { return level; };

	UFUNCTION(BlueprintCallable)
	void ResetGameInstance() { score = 0; lives = 2; level = 1; };

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnScoreChanged OnScoreChanged;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnLivesChanged OnLivesChanged;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnGameOver OnGameOver;
};

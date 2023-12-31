// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PacManGameInstance.generated.h"

// Event for Score increment
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScoreChanged, int, ScoreToAdd);
// Event for Lives increment
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLivesChanged, int, NewLives);

/**
 * 
 */
UCLASS()
class PAC_MAN_PROJECT_API UPacManGameInstance : public UGameInstance
{
	GENERATED_BODY()

	int64 score = 0;
	int lives = 2;

public:

	UFUNCTION(BlueprintCallable)
	void AddScore(int scoreToAdd);

	UFUNCTION(BlueprintCallable, Category = "Score")
	int64 GetScore() { return score; };

	UFUNCTION(BlueprintCallable, Category = "Score")
	void ResetScore() { score = 0; };

	UFUNCTION(BlueprintCallable, Category = "Lives")
	void SetLives(int NewLives) { lives = NewLives; };
	UFUNCTION(BlueprintCallable, Category = "Lives")
	int GetLives() { return lives; };

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnScoreChanged OnScoreChanged;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnLivesChanged OnLivesChanged;
};

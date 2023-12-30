// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PacManGameInstance.generated.h"

// Event for Score increment
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScoreChanged, int, NewScore);

/**
 * 
 */
UCLASS()
class PAC_MAN_PROJECT_API UPacManGameInstance : public UGameInstance
{
	GENERATED_BODY()

	int64 score = 0;

public:
	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddScore(int scoreToAdd);

	UFUNCTION(BlueprintCallable, Category = "Score")
	int64 GetScore() { return score; };

	UFUNCTION(BlueprintCallable, Category = "Score")
	void ResetScore() { score = 0; };

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnScoreChanged OnScoreChanged;
};

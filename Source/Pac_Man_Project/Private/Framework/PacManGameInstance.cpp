// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/PacManGameInstance.h"

void UPacManGameInstance::AddScore(int scoreToAdd)
{
	if ((score + scoreToAdd) % bonusLifeThreshold < score % bonusLifeThreshold) {
		lives++;
		OnLivesChanged.Broadcast(lives);
	}

	score += scoreToAdd;
	OnScoreChanged.Broadcast(score);

}

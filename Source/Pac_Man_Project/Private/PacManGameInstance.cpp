// Fill out your copyright notice in the Description page of Project Settings.


#include "PacManGameInstance.h"

void UPacManGameInstance::AddScore(int scoreToAdd)
{
	score += scoreToAdd;
	OnScoreChanged.Broadcast(score);

}

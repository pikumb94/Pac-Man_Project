// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/PacManGameInstance.h"


void UPacManGameInstance::AddScore(int scoreToAdd, bool isScoreIncremental)
{
	if ((score + scoreToAdd) % bonusLifeThreshold < score % bonusLifeThreshold) {
		SetLives(lives + 1);
	}

	if (isScoreIncremental)
	{
		//When ghosts are eaten is succession the score is incremental
		/*1 in succession - 200 points.
			2 in succession - 400 points.
			3 in succession - 800 points.
			4 in succession - 1600 points.
			5 in succession - 3200 points.
			...and so on*/

		scoreToAdd = FMath::Pow(2.f, IncrementalValue) * (scoreToAdd);
		IncrementalValue++;

		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("Aggiunti: %d punti"), scoreToAdd));

	}

	score += scoreToAdd;


	OnScoreChanged.Broadcast(score);

}

FLevelParamsStruct* UPacManGameInstance::GetCurrentLevelParams()
{
	auto RowMap = LevelProgressionTable->GetRowMap();

	FLevelParamsStruct* LowestLevelRowInTable = nullptr;
	int LowestLevelNumberInTable = 0;

	for (auto& Element: RowMap)
	{
		auto CurrentElementRow = reinterpret_cast<FLevelParamsStruct*>(Element.Value);
		FString TempString =  Element.Key.ToString();

		if (TempString.RemoveFromStart("Level_", ESearchCase::IgnoreCase)) {
			auto RowGameLevel = FCString::Atoi(*TempString);

			if (RowGameLevel > LowestLevelNumberInTable && RowGameLevel <= level) {
				LowestLevelRowInTable = CurrentElementRow;
				LowestLevelNumberInTable = RowGameLevel;

				if (level == RowGameLevel)
					break;


			}

		}
		
	}


	return LowestLevelRowInTable;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "PacManGameMode.h"

#include "PacManController.h"
#include "GridPawn.h"
#include "PickableActor.h"
#include "Kismet/GameplayStatics.h"
#include "PacManGameInstance.h"

APacManGameMode::APacManGameMode()
{
	/* Assign the class types used by this gamemode */
	PlayerControllerClass = APacManController::StaticClass();
	DefaultPawnClass = AGridPawn::StaticClass();


}

void APacManGameMode::StartPlay()
{
	Super::StartPlay();

	TArray<TObjectPtr<AActor>> AllPickableScores;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APickableActor::StaticClass(), AllPickableScores);

	remainingScorePellets = AllPickableScores.Num();


	TObjectPtr<UPacManGameInstance> GI = GetWorld()->GetGameInstance<UPacManGameInstance>();

	if (GI)
	{
		GI->OnScoreChanged.AddDynamic(this, &APacManGameMode::CheckLevelCompleted);
	}

}

void APacManGameMode::CheckLevelCompleted(int ScoreToAdd)
{
	remainingScorePellets--;

	if (remainingScorePellets <= 0)
	{
		UE_LOG(LogTemp, Log, TEXT("PROSSIMO LIVELLO"));
		UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);

	}
}

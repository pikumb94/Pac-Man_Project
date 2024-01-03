// Fill out your copyright notice in the Description page of Project Settings.


#include "PacManGameMode.h"

#include "PacManController.h"
#include "GridPawn.h"
#include "PickableActor.h"
#include "Kismet/GameplayStatics.h"
#include "PacManGameInstance.h"
#include "PacManEnemyAIController.h"
#include "EnemyGridPawn.h"
#include "EnemyDataAsset.h"

APacManGameMode::APacManGameMode()
{
	/* Assign the class types used by this gamemode */
	PlayerControllerClass = APacManController::StaticClass();
	DefaultPawnClass = AGridPawn::StaticClass();
	
	static ConstructorHelpers::FObjectFinder<UEnemyDataAsset> PacManDataAssetRef(TEXT("/ Script / Pac_Man_Project.EnemyDataAsset'/Game/Data/PacManEnemies.PacManEnemies'"));
	if(PacManDataAssetRef.Succeeded())
		EnemiesData = PacManDataAssetRef.Object;

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

	SpawnEnemies();
}

void APacManGameMode::SpawnEnemies()
{
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.bNoFail = true;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	
	for (auto enemyType : TEnumRange<EEnemyType>())
	{
		//EEnemyType enemyType = EEnemyType::Blinky;
		TObjectPtr<APacManEnemyAIController> EnemyController = GetWorld()->SpawnActor<APacManEnemyAIController>();
		EnemyController->SetEnemyType(enemyType);

		TObjectPtr<AEnemyGridPawn> EnemyPawn = GetWorld()->SpawnActor<AEnemyGridPawn>(EnemyPawnClass, EnemiesData->GetEnemyInitialCell(enemyType), FRotator::ZeroRotator);
		EnemyPawn->InitMaterial(EnemiesData->GetEnemyMaterialColor(enemyType));

		EnemyController->Possess(EnemyPawn);

	}

}

void APacManGameMode::CheckLevelCompleted(int ScoreToAdd)
{
	remainingScorePellets--;

	if (remainingScorePellets <= 0)
	{
		ReloadLevel();
	}
}

void APacManGameMode::ReloadLevel(bool bReduceLife)
{
	TObjectPtr<UPacManGameInstance> GI = GetWorld()->GetGameInstance<UPacManGameInstance>();

	if (GI)
	{
		if (bReduceLife)
		{
			//player collided with the enemy, reduce by 1 the lives and reload the same level
			int remainingLives = GI->GetLives() -1;

			if (remainingLives <= 0)
			{
				GI->OnGameOver.Broadcast();
			}
			else
			{
				GI->SetLives(remainingLives - 1);
				UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
			}
			
		}
		else {
			GI->IncrementLevel();
			UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
		}

	}

}

void APacManGameMode::TriggerFrightened()
{
	if (GetWorldTimerManager().IsTimerActive(FrightenedTimerHandle))
	{
		GetWorldTimerManager().ClearTimer(FrightenedTimerHandle);
	}


	OnFrightenedChanged.Broadcast(true);

	GetWorldTimerManager().SetTimer(FrightenedTimerHandle, [this]() {

		OnFrightenedChanged.Broadcast(false);
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("FRIGHT FINITA!")));

	}, FrightenedModeDuration, false);
}

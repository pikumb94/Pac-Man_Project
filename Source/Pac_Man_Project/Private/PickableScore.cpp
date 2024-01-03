// Fill out your copyright notice in the Description page of Project Settings.


#include "PickableScore.h"
#include "PacManGameMode.h"

void APickableScore::OnPickedAction(AActor* Other)
{
	TObjectPtr<APacManGameMode> GM = Cast<APacManGameMode>(GetWorld()->GetAuthGameMode());

	if(GM)
	{
		GM->AddScoreFwd(scoreBaseValue);
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "PickableScore.h"
#include "PacManGameInstance.h"

void APickableScore::OnPickedAction(AActor* Other)
{
	TObjectPtr<UPacManGameInstance> GI = GetWorld()->GetGameInstance<UPacManGameInstance>();

	if(GI)
	{
		GI->AddScore(scoreBaseValue);
	}
}

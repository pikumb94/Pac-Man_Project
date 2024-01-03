// Fill out your copyright notice in the Description page of Project Settings.


#include "PickableEnergizer.h"
#include "PacManGameMode.h"

void APickableEnergizer::OnPickedAction(AActor* Other)
{
	TObjectPtr<APacManGameMode> GM = Cast<APacManGameMode>(GetWorld()->GetAuthGameMode());

	if (GM)
	{
		GM->TriggerFrightened();
		GM->AddScoreFwd(energizerBaseValue);

	}
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickableActor.h"
#include "PickableEnergizer.generated.h"

/**
 * The energizer will trigger the event to change all the enemies in the ghost mode
 */
UCLASS()
class PAC_MAN_PROJECT_API APickableEnergizer : public APickableActor
{
	GENERATED_BODY()

	//Score value of the energizer pickable
	UPROPERTY(EditDefaultsOnly)
	int energizerBaseValue = 50;

protected:
	//Overriden picked action
	virtual void OnPickedAction(AActor* Other) override;
};

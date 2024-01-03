// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickableActor.h"
#include "PickableScore.generated.h"

/**
 * Is a pickable actor that assigns the score to the player when
 * he/she overlaps with it
 */

UCLASS()
class PAC_MAN_PROJECT_API APickableScore : public APickableActor
{
	GENERATED_BODY()

	//Score value of the pickable
	UPROPERTY(EditDefaultsOnly)
	int scoreBaseValue = 10;

protected:
	//Overriden picked action
	virtual void OnPickedAction(AActor* Other) override;
};

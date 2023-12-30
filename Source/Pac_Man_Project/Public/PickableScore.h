// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickableActor.h"
#include "PickableScore.generated.h"

/**
 * 
 */
UCLASS()
class PAC_MAN_PROJECT_API APickableScore : public APickableActor
{
	GENERATED_BODY()
	

protected:

	virtual void OnPickedAction(AActor* Other) override;
};

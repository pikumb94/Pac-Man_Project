// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickableActor.generated.h"

/**
 * It generalizes a pickable actor: an actor that when overlaps with the player performs
 *  a specific action and is destroyed soon after (e.g. coin, score, power-up) 
 */

UCLASS()
class PAC_MAN_PROJECT_API APickableActor : public AActor
{
	GENERATED_BODY()
	
	// Mesh component
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> MeshComponent;


public:	
	// Sets default values for this actor's properties
	APickableActor();

	//Handler for the begin overlap event
	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:

	virtual void OnPickedAction(AActor* Other) {};

};

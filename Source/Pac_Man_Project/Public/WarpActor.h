// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WarpActor.generated.h"


/**
 * This actor is responsible for the teleport effect
 * of PacMan during the traversal of the mid horizontal corridor
 */
UCLASS()
class PAC_MAN_PROJECT_API AWarpActor : public AActor
{
	GENERATED_BODY()
	
	// Collider component of the warp
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UBoxComponent> ColliderComponent;

public:	
	// Sets default values for this actor's properties
	AWarpActor();

	//The target destination where PacMan will be teleported
	UPROPERTY(EditAnywhere)
	TObjectPtr<AWarpActor> WarpActorDestination = nullptr;

	//Handler triggered when the player actor overlaps with the warp, it will be teleported to WarpActorDestination
	UFUNCTION()
	void OnWarpEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};

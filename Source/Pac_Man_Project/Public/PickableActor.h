// A pickable actor is an actor (e.g. coin, score, power-ip) that when collides 
// with the player performs a specific action and is destroyed soon after

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickableActor.generated.h"

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

	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnPickedAction(AActor* Other) {};

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

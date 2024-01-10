// Fill out your copyright notice in the Description page of Project Settings.


#include "PickableActor.h"

#include "GridUtilities.h"

// Sets default values
APickableActor::APickableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	MeshComponent->OnComponentBeginOverlap.AddDynamic(this, &APickableActor::OnBeginOverlap);

	SetRootComponent(MeshComponent);

}

void APickableActor::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OnPickedAction(Other);
	Destroy();
}

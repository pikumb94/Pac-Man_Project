// Fill out your copyright notice in the Description page of Project Settings.


#include "PickableActor.h"

#include "GridUtilities.h"

// Sets default values
APickableActor::APickableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	//The Collider for this actor will be a simplified box collision rounding the mesh
	MeshComponent->SetCollisionProfileName(TEXT("OverlapAll"));
	//MeshComponent->OnComponentHit.AddDynamic(this, &AGridPawn::OnComponentHit);

	SetRootComponent(MeshComponent);

}

// Called when the game starts or when spawned
void APickableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


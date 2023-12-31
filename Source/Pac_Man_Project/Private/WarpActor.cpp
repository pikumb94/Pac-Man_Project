// Fill out your copyright notice in the Description page of Project Settings.


#include "WarpActor.h"

#include "Components/BoxComponent.h"
#include "GridUtilities.h"


// Sets default values
AWarpActor::AWarpActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ColliderComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	ColliderComponent->SetCollisionProfileName(TEXT("OverlapAll"));
	ColliderComponent->OnComponentBeginOverlap.AddDynamic(this, &AWarpActor::OnWarpEnter);

	SetRootComponent(ColliderComponent);

}

// Called when the game starts or when spawned
void AWarpActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWarpActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void AWarpActor::OnWarpEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//void AWarpActor::OnWarpEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && WarpActorDestination)
	{
		OtherActor->SetActorLocation(WarpActorDestination->GetActorLocation()+ WarpActorDestination->GetActorForwardVector() * GridConstants::GridSize);

	}
}


// Fill out your copyright notice in the Description page of Project Settings.

#include "GridPawn.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "GridUtilities.h"

// Sets default values
AGridPawn::AGridPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);


	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement"));
	MovementComponent->MaxSpeed = GridVelocity;

	SetRootComponent(MeshComponent);

}

// Called when the game starts or when spawned
void AGridPawn::BeginPlay()
{
	Super::BeginPlay();

	//LastInputGridPosition = FVector::random;
}

// Called every frame
void AGridPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MovementComponent->AddInputVector(CurrentDirection);
}

// Called to bind functionality to input
void AGridPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AGridPawn::SetDirection(const FVector NewDirection)
{
	//As the Pacman game accept the new direction if and only if you are aligned column/row aligned
	//To check this we do a simple raycast of the size of the pawn

	FHitResult Hit;
	//
	//FVector TraceStart = GetActorLocation()+ NewDirection *150.f+ NewDirection.RotateAngleAxis(90.f,FVector::UpVector)*60.f;
	//FVector TraceEnd = GetActorLocation() + NewDirection * 150.f + NewDirection.RotateAngleAxis(-90.f, FVector::UpVector)*60.f;

	FVector TraceStart = GetActorLocation();
	FVector TraceEnd = GetActorLocation() + NewDirection * (GridConstants::GridSize * 0.75f);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_WorldStatic, QueryParams);



	if (Hit.bBlockingHit && IsValid(Hit.GetActor()))
	{
		UE_LOG(LogTemp, Log, TEXT("Trace hit actor: %s"), *Hit.GetActor()->GetName());
		DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 5.f, 0, 10.0f);
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("No Actors were hit"));


		// If the pawn is cornering, snap it on the grid
		bool isCornering = FVector::DotProduct(CurrentDirection, NewDirection) == 0.f;
		if (isCornering) {

			if (LastInputGridPosition == VectorGridSnap(GetActorLocation())) {
				return;
			}
			SetActorLocation(VectorGridSnap(GetActorLocation()));
			/*
			auto CurrPawnLocation = GetActorLocation();


			if (FVector::DotProduct(NewDirection, FVector::ForwardVector) == 0)
				//if it is moving along the Y axis, snap on the X
				SetActorLocation(FVector(FMath::GridSnap(CurrPawnLocation.X, GridConstants::GridSize), CurrPawnLocation.Y, CurrPawnLocation.Z));

			else
				//if it is moving along the X axis, snap on the Y
				SetActorLocation(FVector(CurrPawnLocation.X, FMath::GridSnap(CurrPawnLocation.Y, GridConstants::GridSize), CurrPawnLocation.Z));
			*/
		}

		LastInputGridPosition = VectorGridSnap(GetActorLocation());
		//Update the new current Direction

		CurrentDirection = NewDirection;

	}



}

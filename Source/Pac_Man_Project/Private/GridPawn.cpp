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
	MeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

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

	//In a more light version we can use this but raycast should be used to replace normal collision detection
	//SetActorLocation(GetActorLocation() + CurrentDirection * GridVelocity * DeltaTime);
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

		}

		LastInputGridPosition = VectorGridSnap(GetActorLocation());

		//Update the new current Direction
		CurrentDirection = NewDirection;
	}



}

void AGridPawn::ForceDirection(const FVector NewDirection)
{
	if (FVector::DotProduct(CurrentDirection, NewDirection) == 0.f)
		SetActorLocation(VectorGridSnap(GetActorLocation()));

	CurrentDirection = NewDirection;


}

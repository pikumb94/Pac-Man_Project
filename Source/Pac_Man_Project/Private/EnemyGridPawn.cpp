// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyGridPawn.h"


AEnemyGridPawn::AEnemyGridPawn()
{
	//MeshComponent->OnComponentHit.AddDynamic(this, &AEnemyGridPawn::OnComponentHit);
	MeshComponent->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
	MeshComponent->OnComponentBeginOverlap.AddDynamic(this, &AEnemyGridPawn::OnEnemyOverlap);
}


void AEnemyGridPawn::BeginPlay()
{
	Super::BeginPlay();

	SetDirection(FVector::ForwardVector);
}


void AEnemyGridPawn::OnEnemyOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%s ha sbattuto su %s"), *OverlappedComponent->GetName(), *OtherComp->GetName()));
}


/*
void AEnemyGridPawn::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//USALO DOPO PER CAPIRE QUANDO SBATTE SUL MURO
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%s ha sbattuto su %s"), *HitComponent->GetName(), *OtherActor->GetName()));
}*/
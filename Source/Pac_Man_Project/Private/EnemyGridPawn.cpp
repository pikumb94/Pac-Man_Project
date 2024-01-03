// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyGridPawn.h"

#include "Kismet/GameplayStatics.h"
#include "PacManGameMode.h"
#include "GridUtilities.h"

AEnemyGridPawn::AEnemyGridPawn()
{
	//MeshComponent->OnComponentHit.AddDynamic(this, &AEnemyGridPawn::OnComponentHit);
	MeshComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	MeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Ignore);
	MeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);

	MeshComponent->SetCollisionProfileName(TEXT("BlockAll"));
	MeshComponent->OnComponentBeginOverlap.AddDynamic(this, &AEnemyGridPawn::OnEnemyOverlap);
	MeshComponent->OnComponentHit.AddDynamic(this, &AEnemyGridPawn::OnComponentHit);

}


void AEnemyGridPawn::BeginPlay()
{
	Super::BeginPlay();
}


void AEnemyGridPawn::OnEnemyOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%s ha sbattuto su %s"), *OverlappedComponent->GetName(), *OtherComp->GetName()));

	if(UGameplayStatics::GetPlayerPawn(GetWorld(),0) == OtherActor)
	{
		TObjectPtr<APacManGameMode> GM = Cast<APacManGameMode>(GetWorld()->GetAuthGameMode());

		if (GM) {
			GM->ReloadLevel(true);
		}
	}
}



void AEnemyGridPawn::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//USALO DOPO PER CAPIRE QUANDO SBATTE SUL MURO
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%s ha sbattuto su %s"), *HitComponent->GetName(), *OtherActor->GetName()));
	SetActorLocation(VectorGridSnap(GetActorLocation()));

}
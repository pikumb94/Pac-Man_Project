// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyGridPawn.h"

#include "Kismet/GameplayStatics.h"
#include "PacManEnemyAIController.h"
#include "GridUtilities.h"

AEnemyGridPawn::AEnemyGridPawn()
{
	MeshComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	MeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Ignore);
	MeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);

	MeshComponent->SetCollisionProfileName(TEXT("BlockAll"));
	MeshComponent->OnComponentBeginOverlap.AddDynamic(this, &AEnemyGridPawn::OnEnemyOverlap);

	//CON IL GIUSTO COLLIDER NON SERVE
	//MeshComponent->OnComponentHit.AddDynamic(this, &AEnemyGridPawn::OnComponentHit);

}



void AEnemyGridPawn::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemyGridPawn::InitMaterial(FColor EnemyColor)
{
	EnemyMaterialInstance = UMaterialInstanceDynamic::Create(MeshComponent->GetMaterial(0), nullptr);
	MeshComponent->SetMaterial(0, EnemyMaterialInstance);

	EnemyMaterialInstance->SetVectorParameterValue(TEXT("BaseColor"), EnemyColor);

}

void AEnemyGridPawn::SetBlinkEffectMaterial(bool bIsFrightened)
{
	EnemyMaterialInstance->SetScalarParameterValue(TEXT("IsFrightened"), (bIsFrightened? 1.f:0.f));
}

void AEnemyGridPawn::SetOpacityMaterial(float opacityValue)
{
	EnemyMaterialInstance->SetScalarParameterValue(TEXT("Opacity"), opacityValue);
}

void AEnemyGridPawn::OnEnemyOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%s ha overlap su %s"), *OverlappedComponent->GetName(), *OtherComp->GetName()));

	if(UGameplayStatics::GetPlayerPawn(GetWorld(),0) == OtherActor)
	{
		//Player overlapped with Enemy
		TObjectPtr<APacManEnemyAIController> AIEnemyController = Cast<APacManEnemyAIController>(GetController());
		
		if (AIEnemyController)
			AIEnemyController->PawnOverlappedPlayerHandler();
	}
}



void AEnemyGridPawn::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//USALO DOPO PER CAPIRE QUANDO SBATTE SUL MURO
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("%s ha HIT su %s"), *HitComponent->GetName(), *OtherActor->GetName()));
	DrawDebugCircle(GetWorld(), Hit.ImpactPoint, 25, 25, FColor::Red, false, 1, 0, 0, FVector::RightVector, FVector::ForwardVector);

	//SetActorLocation(VectorGridSnap(GetActorLocation()));

}

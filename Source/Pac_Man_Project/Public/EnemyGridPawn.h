// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridPawn.h"
#include "EnemyGridPawn.generated.h"

/**
 * 
 */
UCLASS()
class PAC_MAN_PROJECT_API AEnemyGridPawn : public AGridPawn
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	TObjectPtr<UMaterialInstanceDynamic> EnemyMaterialInstance;

public:
	// Sets default values for this pawn's properties
	AEnemyGridPawn();

	void InitMaterial(FColor EnemyColor);

	void FrightenedBlinkMaterial(bool bIsFrightened);

	UFUNCTION()
	void OnEnemyOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	/*
	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	*/
};

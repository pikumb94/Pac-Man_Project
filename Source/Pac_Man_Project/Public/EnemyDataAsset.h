// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EnemyDataAsset.generated.h"

UENUM(BlueprintType)
enum class EEnemyType : uint8 {
	Blinky = 0,		//Shadow - Red
	Pinky,			//Speedy - Pink
	Inky,			//Bashful - Cyan
	Clyde			//Pokey - Orange
};
ENUM_RANGE_BY_FIRST_AND_LAST(EEnemyType, EEnemyType::Blinky, EEnemyType::Clyde);

USTRUCT()
struct FEnemyInfo {
	GENERATED_BODY()

	FEnemyInfo()
		: EnemyColor(FColor::White)
		, ScatterCell(FVector::ZeroVector)
		, InitialCell(FVector::ZeroVector)
	{ };

	//The default color of the enemy
	UPROPERTY(EditAnywhere)
	FColor EnemyColor;

	//The target cell the enemy will try to reach in scatter state
	UPROPERTY(EditAnywhere)
	FVector ScatterCell;

	//The Initial cell where enemy will be spawned
	UPROPERTY(EditAnywhere)
	FVector InitialCell;
};


/**
 * The data for the different enemy types
 */

UCLASS()
class PAC_MAN_PROJECT_API UEnemyDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	TMap<EEnemyType, FEnemyInfo> AssetItems;

public:
	FColor GetEnemyMaterialColor(EEnemyType enemyType) const { return AssetItems[enemyType].EnemyColor; };
	FVector GetEnemyScatterCell(EEnemyType enemyType) const { return AssetItems[enemyType].ScatterCell; };
	FVector GetEnemyInitialCell(EEnemyType enemyType) const { return AssetItems[enemyType].InitialCell; };

};

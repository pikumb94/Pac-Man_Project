// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EnemyDataAsset.generated.h"

/**
 * The data for the different enemy types
 */


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

	UPROPERTY(EditAnywhere)
	FColor EnemyColor;

	UPROPERTY(EditAnywhere)
	FVector ScatterCell;

	UPROPERTY(EditAnywhere)
	FVector InitialCell;
};

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

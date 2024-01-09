// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PacManController.generated.h"

/**
 * The player controller: it simply forwards the movement direction to the AGridPawn
 */

UCLASS()
class PAC_MAN_PROJECT_API APacManController : public APlayerController
{
	GENERATED_BODY()

protected:
    void SetupInputComponent() override;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
    class UInputMappingContext* InputMapping;

    virtual void BeginPlay() override;

public:

    void MovePlayer(const struct FInputActionInstance& InputInstance);

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    class UInputAction* InputMove;

    //All player's updates when enemy state switches
    UFUNCTION()
    void UpdatePlayerBasedOnEnemyState(EEnemyState NewState);
};

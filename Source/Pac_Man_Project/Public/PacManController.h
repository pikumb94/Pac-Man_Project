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

public:
    void MoveUp();
    void MoveDown();
    void MoveLeft();
    void MoveRight();
};

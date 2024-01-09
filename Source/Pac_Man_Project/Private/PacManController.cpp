// Fill out your copyright notice in the Description page of Project Settings.


#include "PacManController.h"

#include "GridPawn.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Framework/PacManGameMode.h"
#include "PacManEnemyAIController.h"

void APacManController::SetupInputComponent() {
    Super::SetupInputComponent();

    // Get the local player subsystem
    UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

    // Clear out existing mapping, and add our mapping
    Subsystem->ClearAllMappings();
    Subsystem->AddMappingContext(InputMapping, 0);

    UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent);

    EIC->BindAction(InputMove, ETriggerEvent::Triggered, this, &APacManController::MovePlayer);

}

void APacManController::BeginPlay()
{
    Super::BeginPlay();

    if (TObjectPtr<APacManGameMode> GM = Cast<APacManGameMode>(GetWorld()->GetAuthGameMode())) {
        GM->OnChangeState.AddDynamic(this, &APacManController::UpdatePlayerBasedOnEnemyState);
    }
}

void APacManController::MovePlayer(const FInputActionInstance& InputInstance)
{
    FVector2D AxisInput2D = InputInstance.GetValue().Get<FVector2D>();
    if (auto PacManPawn = Cast<AGridPawn>(GetPawn())) {
        AxisInput2D.Y *= -1.0;
        PacManPawn->SetDirection(FVector(AxisInput2D,0));
    }
}

void APacManController::UpdatePlayerBasedOnEnemyState(EEnemyState NewState)
{
    if (auto PlayerPacManPawn = Cast<AGridPawn>(GetPawn())) {
        if (NewState == EEnemyState::Frightened) {
            PlayerPacManPawn->SetToAlteredVelocity();
        }
        else {
            PlayerPacManPawn->ResetGridVelocity();
        }
    }

}

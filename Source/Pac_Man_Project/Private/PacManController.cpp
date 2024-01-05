// Fill out your copyright notice in the Description page of Project Settings.


#include "PacManController.h"
#include "GridPawn.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

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

void APacManController::MovePlayer(const FInputActionInstance& InputInstance)
{
    FVector2D AxisInput2D = InputInstance.GetValue().Get<FVector2D>();
    if (auto PacManPawn = Cast<AGridPawn>(GetPawn())) {
        AxisInput2D.Y *= -1.0;
        PacManPawn->SetDirection(FVector(AxisInput2D,0));
    }
}

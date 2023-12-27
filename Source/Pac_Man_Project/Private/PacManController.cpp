// Fill out your copyright notice in the Description page of Project Settings.


#include "PacManController.h"
#include "GridPawn.h"

void APacManController::SetupInputComponent() {
    Super::SetupInputComponent();
    InputComponent->BindAction("MoveUp", IE_Pressed, this, &APacManController::MoveUp);
    InputComponent->BindAction("MoveDown", IE_Pressed, this, &APacManController::MoveDown);
    InputComponent->BindAction("MoveLeft", IE_Pressed, this, &APacManController::MoveLeft);
    InputComponent->BindAction("MoveRight", IE_Pressed, this, &APacManController::MoveRight);
}

void APacManController::MoveUp() {
    if (auto PacManPawn = Cast<AGridPawn>(GetPawn())) {
        PacManPawn->SetDirection(FVector::LeftVector);
    }
}

void APacManController::MoveDown() {
    if (auto PacManPawn = Cast<AGridPawn>(GetPawn())) {
        PacManPawn->SetDirection(FVector::RightVector);
    }
}

void APacManController::MoveLeft() {
    if (auto PacManPawn = Cast<AGridPawn>(GetPawn())) {
        PacManPawn->SetDirection(FVector::BackwardVector);
    }
}

void APacManController::MoveRight() {
    if (auto PacManPawn = Cast<AGridPawn>(GetPawn())) {
        PacManPawn->SetDirection(FVector::ForwardVector);
    }
}
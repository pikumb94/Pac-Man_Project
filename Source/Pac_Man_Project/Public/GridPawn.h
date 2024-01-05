

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GridPawn.generated.h"

/**
 * A GridPawn is a pawn moving alongside a grid in a plane constrained to z=0 and of size GridConstants::GridSize
 */

UCLASS()
class PAC_MAN_PROJECT_API AGridPawn : public APawn
{
	GENERATED_BODY()

	// The orthogonal direction the pawn will walk: it will continue until an obstacle/blocking collision is found
	FVector CurrentDirection = FVector::ZeroVector;
	FVector LastInputGridPosition;

	// The velocity of the actor (as unit per second)
	UPROPERTY(EditDefaultsOnly)
	float baseGridVelocity = 600.0f;

	bool isCornering = false;
	
	// Movement component
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr <class UFloatingPawnMovement> MovementComponent;

public:
	// Sets default values for this pawn's properties
	AGridPawn();

	// Getter for the current moving direction of the pawn
	FVector GetMovingDirection() const { return CurrentDirection; };

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Mesh component
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> MeshComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//The Pawn tries to pursue the given direction instantly
	void SetDirection(const FVector NewDirection);

	void ForceDirection(const FVector NewDirection);

	UFUNCTION(BlueprintCallable)
	void SetGridVelocity(float NewMaxGridVelocity);
	UFUNCTION(BlueprintCallable)
	float GetGridVelocity();
	UFUNCTION(BlueprintCallable)
	void ResetGridVelocity();
};

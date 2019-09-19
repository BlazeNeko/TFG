// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "TFGPawn.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/World.h"
#include "Engine/StaticMesh.h"

ATFGPawn::ATFGPawn()
{
	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> PlaneMesh;
		FConstructorStatics()
			: PlaneMesh(TEXT("/Game/Flying/Meshes/UFO.UFO"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	// Create static mesh component
	PlaneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlaneMesh0"));
	PlaneMesh->SetStaticMesh(ConstructorStatics.PlaneMesh.Get());	// Set static mesh
	RootComponent = PlaneMesh;

}

void ATFGPawn::Tick(float DeltaSeconds)
{
	// Call any parent class Tick implementation
	Super::Tick(DeltaSeconds);

}

//Gira el peón hacia la dirección del vector de dirección
void ATFGPawn::rotateTowardsDirection(FVector Direction)
{
	//Interpolar la rotación inicial y la objetivo según la velocidad de rotación del pawn
	FRotator rotation = FMath::RInterpTo
	(
		PlaneMesh->GetComponentTransform().GetRotation().Rotator()  //Rotator del pawn
		, Direction.ToOrientationRotator(),							//Rotator del vector de dirección
		GetWorld()->DeltaTimeSeconds,								//Deltatime
		RotationSpeed												//Velocidad de rotación
	);
	//Establecer la nueva rotación
	PlaneMesh->SetRelativeRotation(rotation);
}

//Mueve el peón hacia la dirección del vector de dirección
void ATFGPawn::moveToDirection(FVector Direction, float speedMultiplier)
{
	//Obtener DeltaSeconds
	float deltaSeconds = GetWorld()->DeltaTimeSeconds;

	//Calcular la nueva posición
	Direction.Normalize();
	FVector distance =  Direction * Speed * speedMultiplier * deltaSeconds;
	FVector newPosition = PlaneMesh->GetComponentTransform().GetLocation() + distance;

	//Establecer la nueva transformación del peón
	//PlaneMesh->SetWorldTransform(FTransform(PlaneMesh->GetComponentTransform().GetRotation(), newPosition, PlaneMesh->GetComponentTransform().GetScale3D()));
	AddMovementInput(Direction, Speed *speedMultiplier * deltaSeconds, false);
}

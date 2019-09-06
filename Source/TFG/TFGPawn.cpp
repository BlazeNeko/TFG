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

//Gira el pe�n hacia la direcci�n del vector de direcci�n
void ATFGPawn::rotateTowardsDirection(FVector Direction)
{
	//Interpolar la rotaci�n inicial y la objetivo seg�n la velocidad de rotaci�n del pawn
	FRotator rotation = FMath::RInterpTo
	(
		PlaneMesh->GetComponentTransform().GetRotation().Rotator()  //Rotator del pawn
		, Direction.ToOrientationRotator(),							//Rotator del vector de direcci�n
		GetWorld()->DeltaTimeSeconds,								//Deltatime
		RotationSpeed												//Velocidad de rotaci�n
	);
	//Establecer la nueva rotaci�n
	PlaneMesh->SetRelativeRotation(rotation);
}

//Mueve el pe�n hacia la direcci�n del vector de direcci�n
void ATFGPawn::moveToDirection(FVector Direction)
{
	//Obtener DeltaSeconds
	float deltaSeconds = GetWorld()->DeltaTimeSeconds;

	//Calcular la nueva posici�n
	Direction.Normalize();
	FVector distance =  Direction * Speed * deltaSeconds;
	FVector newPosition = PlaneMesh->GetComponentTransform().GetLocation() + distance;

	//Establecer la nueva transformaci�n del pe�n
	//PlaneMesh->SetWorldTransform(FTransform(PlaneMesh->GetComponentTransform().GetRotation(), newPosition, PlaneMesh->GetComponentTransform().GetScale3D()));
	AddMovementInput(Direction, Speed *deltaSeconds, false);
}

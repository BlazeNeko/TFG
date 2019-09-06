// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TFGPawn.generated.h"




UCLASS(Config=Game)
class ATFGPawn : public APawn
{
	GENERATED_BODY()

	/** StaticMesh component that will be the visuals for our flying pawn */
	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* PlaneMesh;
	
public:
	ATFGPawn();

	// Begin AActor overrides
	virtual void Tick(float DeltaSeconds) override;
	// End AActor overrides
	/** Rotation speed */
	UPROPERTY(Category = "Speed", EditAnywhere)
	float Speed = 300;
	/** Rotation speed */
	UPROPERTY(Category = "Speed", EditAnywhere)
	float RotationSpeed = 10;

public:
	/** Returns PlaneMesh subobject **/
	FORCEINLINE class UStaticMeshComponent* GetPlaneMesh() const { return PlaneMesh; }

	//Gira el peón hacia la dirección del vector de dirección
	void rotateTowardsDirection(FVector Direction);
	//Mueve el peón hacia la dirección del vector de dirección
	void moveToDirection(FVector Direction);
};

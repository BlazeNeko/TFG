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

	/** Current forward speed */
	UPROPERTY(Category = "Speed", BlueprintReadOnly)
	float CurrentForwardSpeed;


public:
	/** Returns PlaneMesh subobject **/
	FORCEINLINE class UStaticMeshComponent* GetPlaneMesh() const { return PlaneMesh; }

};

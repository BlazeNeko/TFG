// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TFGPawn.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "MyAIController.generated.h"

/**
 * 
 */
UCLASS()
class TFG_API AMyAIController : public AAIController
{
	GENERATED_BODY()
public:

	void calcDirectionToTarget(FBlackboardKeySelector TargetCoordinates, FBlackboardKeySelector MovementDirection);

	bool calcAvoidingMovementDirection(FBlackboardKeySelector MovementDirection);

	bool calcFormationAvoidingMovementDirection(FBlackboardKeySelector FormationAvoidPosition, FBlackboardKeySelector MovementDirection);

	bool calcDirectionToFormationPos(FBlackboardKeySelector Leader, FBlackboardKeySelector FormationPosition, FBlackboardKeySelector MovementDirection);

	void movePawnToDirection(FVector Direction, float speedMultiplier);

	void rotatePawnToDirection(FVector Direction);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "AvoidObstaclesMovement.generated.h"

/**
 * 
 */
UCLASS()
class TFG_API UAvoidObstaclesMovement : public UBTTaskNode
{
	GENERATED_BODY()
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	//Dirección del movimiento
	UPROPERTY(EditAnywhere, Category = "Blackboard")
		struct FBlackboardKeySelector MovementDirection;
};

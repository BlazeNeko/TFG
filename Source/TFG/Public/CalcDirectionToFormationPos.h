// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CalcDirectionToFormationPos.generated.h"

/**
 *
 */
UCLASS()
class TFG_API UCalcDirectionToFormationPos : public UBTTaskNode
{
	GENERATED_BODY()
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	//L�der de la formaci�n
	UPROPERTY(EditAnywhere, Category = "Blackboard")
		struct FBlackboardKeySelector Leader;
	//Posici�n que ocupa el pe�n dentro de la formaci�n
	UPROPERTY(EditAnywhere, Category = "Blackboard")
		struct FBlackboardKeySelector FormationPosition;
	UPROPERTY(EditAnywhere, Category = "Blackboard")
		struct FBlackboardKeySelector MovementDirection;
};

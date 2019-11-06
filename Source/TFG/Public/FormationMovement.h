// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "FormationMovement.generated.h"

/**
 * 
 */
UCLASS()
class TFG_API UFormationMovement : public UBTTaskNode
{
	GENERATED_BODY()
		EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	//Lider de la formacion
	UPROPERTY(EditAnywhere, Category = "Blackboard")
		struct FBlackboardKeySelector Leader;
	//Posicion que ocupa el peon dentro de la formacion
	UPROPERTY(EditAnywhere, Category = "Blackboard")
		struct FBlackboardKeySelector FormationPosition;
	UPROPERTY(EditAnywhere, Category = "Blackboard")
		struct FBlackboardKeySelector MovementDirection;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "ChooseTarget.generated.h"

/**
 * 
 */
UCLASS()
class TFG_API UChooseTarget : public UBTTaskNode
{
	GENERATED_BODY()
public:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
		struct FBlackboardKeySelector Target;
	UPROPERTY(EditAnywhere, Category = "Blackboard")
		struct FBlackboardKeySelector TargetCoordinates;
	UPROPERTY(EditAnywhere, Category = "Blackboard")
		struct FBlackboardKeySelector Index;
	UPROPERTY(EditAnywhere, Category = "Blackboard")
		struct FBlackboardKeySelector IsOnTarget;

};

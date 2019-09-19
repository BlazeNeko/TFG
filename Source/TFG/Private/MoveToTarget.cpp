// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveToTarget.h"
#include "MyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
EBTNodeResult::Type UMoveToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Obtener el controlador de AI
	AMyAIController* AIcontroller = (AMyAIController*)OwnerComp.GetAIOwner();

	//UE_LOG(LogTemp, Warning, TEXT("Direction Set: %s"), *(FString(TEXT(""))))
	//UE_LOG(LogTemp, Warning, TEXT("Direction Set: %s"), *direction.ToString())

	AIcontroller->calcDirectionToTarget(TargetCoordinates, MovementDirection);

	//Aplicar movimiento y rotación
	FVector direction = AIcontroller->GetBlackboardComponent()->GetValueAsVector(MovementDirection.SelectedKeyName);
	AIcontroller->rotatePawnToDirection(direction);
	AIcontroller->movePawnToDirection(direction, 1);
	return EBTNodeResult::Type();
}


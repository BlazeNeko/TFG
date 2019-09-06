// Fill ou// Fill out your copyright notice in the Description page of Project Settings.


#include "CalcDirectionToFormationPos.h"
#include "MyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UCalcDirectionToFormationPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Obtener el controlador de AI
	AMyAIController *AIcontroller = (AMyAIController*) OwnerComp.GetAIOwner();   
	
	//UE_LOG(LogTemp, Warning, TEXT("Direction Set: %s"), *(FString(TEXT(""))))
	//UE_LOG(LogTemp, Warning, TEXT("Direction Set: %s"), *direction.ToString())

	if (!AIcontroller->calcDirectionToFormationPos(Leader, FormationPosition, MovementDirection)) {
		return EBTNodeResult::Failed;
	}

	//Aplicar movimiento y rotación
	FVector direction = AIcontroller->GetBlackboardComponent()->GetValueAsVector(MovementDirection.SelectedKeyName);
	AIcontroller->rotatePawnToDirection(direction);
	AIcontroller->movePawnToDirection(direction);
	return EBTNodeResult::Type();
}

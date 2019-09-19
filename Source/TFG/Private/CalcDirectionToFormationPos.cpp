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

	FVector direction = AIcontroller->GetBlackboardComponent()->GetValueAsVector(MovementDirection.SelectedKeyName);
	FVector position = AIcontroller->GetBlackboardComponent()->GetValueAsVector(FormationPosition.SelectedKeyName);

	//Aplicar movimiento y rotación
	float speedMultiplier = 1;
	//If distancia del pawn respecto a la posición que debe alcanzar en la formación > 150, se aplica un speed multiplier
	if (FVector::Distance(AIcontroller->GetPawn()->GetTransform().GetLocation(), position) > 150)
		speedMultiplier = 1.5;
	AIcontroller->rotatePawnToDirection(direction);
	AIcontroller->movePawnToDirection(direction, speedMultiplier);
	return EBTNodeResult::Type();
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "FormationAvoidObstaclesMovement.h"

#include "MyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"




EBTNodeResult::Type UFormationAvoidObstaclesMovement::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Obtener el controlador de AI
	AMyAIController* AIcontroller = (AMyAIController*)OwnerComp.GetAIOwner();

	//UE_LOG(LogTemp, Warning, TEXT("Direction Set: %s"), *(FString(TEXT(""))))
	//UE_LOG(LogTemp, Warning, TEXT("Direction Set: %s"), *direction.ToString())

	if (!AIcontroller->calcFormationAvoidingMovementDirection(FormationAvoidPosition, MovementDirection)) {
		return EBTNodeResult::Failed;
	}

	FVector direction = AIcontroller->GetBlackboardComponent()->GetValueAsVector(MovementDirection.SelectedKeyName);
	FVector position = AIcontroller->GetBlackboardComponent()->GetValueAsVector(FormationAvoidPosition.SelectedKeyName);

	float speedMultiplier = 1;
	//If distancia del pawn respecto a la posición que debe alcanzar en la formación > 150, se aplica un speed multiplier
	if (FVector::Distance(AIcontroller->GetPawn()->GetTransform().GetLocation(), AIcontroller->GetBlackboardComponent()->GetValueAsVector(FormationAvoidPosition.SelectedKeyName)) > 150)
		speedMultiplier = 1.5;

	//Aplicar movimiento y rotación

	AIcontroller->rotatePawnToDirection(direction);
	AIcontroller->movePawnToDirection(direction, speedMultiplier);
	return EBTNodeResult::Type();
}

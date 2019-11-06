// Fill out your copyright notice in the Description page of Project Settings.


#include "FormationMovement.h"
#include "MyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UFormationMovement::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Obtener el controlador de AI
	AMyAIController* AIcontroller = (AMyAIController*)OwnerComp.GetAIOwner();

	//UE_LOG(LogTemp, Warning, TEXT("Direction Set: %s"), *(FString(TEXT(""))))
	//UE_LOG(LogTemp, Warning, TEXT("Direction Set: %s"), *direction.ToString())

	if (!AIcontroller->calcDirectionToFormationPos(Leader, FormationPosition, MovementDirection)) {
		return EBTNodeResult::Failed;
	}

	FVector direction = AIcontroller->GetBlackboardComponent()->GetValueAsVector(MovementDirection.SelectedKeyName);
	FVector position = AIcontroller->GetBlackboardComponent()->GetValueAsVector(FormationPosition.SelectedKeyName);

	//Aplicar movimiento y rotaci�n
	float speedMultiplier = 1;
	//If distancia del pawn respecto a la posici�n que debe alcanzar en la formaci�n > 50, se aplica un speed multiplier
	if (FVector::Distance(AIcontroller->GetPawn()->GetTransform().GetLocation(), position) > 100)
		speedMultiplier = 1.5;
	AIcontroller->rotatePawnToDirection(direction);
	AIcontroller->movePawnToDirection(direction, speedMultiplier);
	return EBTNodeResult::Type();
}

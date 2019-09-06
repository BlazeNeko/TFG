// Fill out your copyright notice in the Description page of Project Settings.

#include "MyAIController.h"
#include "ConstructorHelpers.h"
#include "FormationComponent.h"
#include "SensorComponent.h"
#include "BehaviorTree/BlackboardComponent.h"



void AMyAIController::calcDirectionToTarget(FBlackboardKeySelector TargetCoordinates, FBlackboardKeySelector MovementDirection) {

	auto pawn = GetPawn();           //Pe�n controlado por el controlador


	//Calculamos la direcci�n a seguir
	auto bbcomp = GetBlackboardComponent();
	FVector goal = bbcomp->GetValueAsVector(TargetCoordinates.SelectedKeyName);
	FVector direction = goal - pawn->GetTransform().GetLocation();

	//Guardamos el resultado en la pizarra
	bbcomp->SetValueAsVector(MovementDirection.SelectedKeyName, direction);
}

bool AMyAIController::calcAvoidingMovementDirection(FBlackboardKeySelector MovementDirection) {

	auto pawn = GetPawn();           //Pe�n controlado por el controlador

	auto sensorComp = pawn->FindComponentByClass<USensorComponent>();  //componente de ruta del pe�n

	if (!ensure(sensorComp)) {   //Ensure es una macro especial de Unreal Engine para verificar que se cumple algo. 
		return false;
	}
	//Leemos los datos relevantes del sensor
	FString direction = sensorComp->direction;  //Dirección del sensor que detectó el obstáculo. H = Horizontal. V = Vertical.
	FVector normalVector = sensorComp->frontHitNormal;  //Vector normal respecto a la superficie golpeada por el rayo que impactó con el obstáculo más próximo
	int32 way = sensorComp->way;				//+1 o -1. Multiplica a la dirección a seguir según si se ha detectado el obstáculo arriba(1), izquierda(1), abajo(-1), derecha(-1)
	
	FVector upVector = pawn->GetActorUpVector();		//Vector arriba respecto al pawn
	FVector rightVector = pawn->GetActorRightVector();	//Vector derecha respecto al pawn

	//Calculamos la dirección necesaria para evitar el obstáculo. Prod vectorial entre upVector o rightVector y normalVector = dirección para evitar el obstáculo
	FVector directionVector;
	if (direction == "H") {
		directionVector = FVector::CrossProduct(normalVector, upVector) * way;
	}
	else {
		directionVector = FVector::CrossProduct(normalVector, rightVector) * way;
	}

	//Guardamos resultado en la pizarra
	auto bbcomp = GetBlackboardComponent();
	bbcomp->SetValueAsVector(MovementDirection.SelectedKeyName, directionVector);

	return true;
}

bool AMyAIController::calcDirectionToFormationPos(FBlackboardKeySelector Leader, FBlackboardKeySelector FormationPosition, FBlackboardKeySelector MovementDirection) {

	auto pawn = GetPawn();           //Pe�n controlado por el controlador
	auto formationComp = pawn->FindComponentByClass<UFormationComponent>();  //componente de ruta del pe�n

	if (!ensure(formationComp)) {   //Ensure es una macro especial de Unreal Engine para verificar que se cumple algo. 
		return false;
	}

	//Obtenemos el offset del pe�n respecto al l�der de la formaci�n
	auto formationOffsets = formationComp->getFormationOffsets();
	auto formationPositions = formationComp->getPawnsInFormation();
	FVector offset = formationOffsets[formationPositions.Find(pawn)];

	//Calculamos la direcci�n a seguir
	auto bbcomp = GetBlackboardComponent();
	AActor* formationLeader = (AActor*)bbcomp->GetValueAsObject(Leader.SelectedKeyName);   //Líder de la formación
	FVector targetLocation = formationLeader->GetTransform().GetLocation() + 			   //Posición del líder + offset de la posición
								formationLeader->GetTransform().GetRotation().RotateVector(offset);	//rotamos el offset según la rotación del líder


	FVector direction = targetLocation - pawn->GetTransform().GetLocation();				//Dirección del movimiento


	//Guardamos los resultado en la pizarra
	bbcomp->SetValueAsVector(MovementDirection.SelectedKeyName, direction);
	bbcomp->SetValueAsVector(FormationPosition.SelectedKeyName, targetLocation);

	return true;
}

void AMyAIController::movePawnToDirection(FVector Direction)
{
	ATFGPawn *pawn = (ATFGPawn *) GetPawn();
	pawn->moveToDirection(Direction);
}

void AMyAIController::rotatePawnToDirection(FVector Direction)
{
	ATFGPawn* pawn = (ATFGPawn*)GetPawn();
	pawn->rotateTowardsDirection(Direction);
}

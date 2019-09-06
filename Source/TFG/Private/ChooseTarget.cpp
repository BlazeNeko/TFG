// Fill out your copyright notice in the Description page of Project Settings.

#include "ChooseTarget.h"
#include "AIController.h"
#include "PathComponent.h"
#include "BehaviorTree/BlackboardComponent.h"


EBTNodeResult::Type UChooseTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Obtener la ruta a seguir
	auto AIcontroller = OwnerComp.GetAIOwner();   //Controlador de AI
    auto pawn = AIcontroller->GetPawn();           //Peón controlado por el controlador
	auto pathComp = pawn->FindComponentByClass<UPathComponent>();  //componente de ruta del peón
    //Comprobar que la ruta no está vacía
	if (!ensure(pathComp)) {   //Ensure es una macro especial de Unreal Engine para verificar que se cumple algo. 
        return EBTNodeResult::Failed; 
    }
	//Evitar rutas vacias
	auto path = pathComp->getPathArray();
	if (path.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Se necesita añadir puntos a la ruta"));
		return EBTNodeResult::Failed;
	}
	//
    //Obtener el siguiente punto de ruta
	//
	auto bbcomp = OwnerComp.GetBlackboardComponent();
	int32 index = bbcomp->GetValueAsInt(Index.SelectedKeyName);
	bbcomp->SetValueAsObject(Target.SelectedKeyName, path[index]);
    bbcomp->SetValueAsVector(TargetCoordinates.SelectedKeyName, path[index]->GetActorLocation());
    //Actualizar índices
    int32 next = (index + 1) % path.Num();
    bbcomp->SetValueAsInt(Index.SelectedKeyName, next);

	//cambiar IsOnTarget a False
	bbcomp->SetValueAsBool(IsOnTarget.SelectedKeyName, false);

	return EBTNodeResult::Succeeded;
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "FormationComponent.h"


//Declarations of static members
TArray<FVector> UFormationComponent::formationOffsets;
TArray<APawn*> UFormationComponent::pawnsInFormation;

// Sets default values for this component's properties
UFormationComponent::UFormationComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

UFormationComponent::~UFormationComponent()
{
	destroyFormationOffsets();
	destroyPawnsInFormation();
}

//Añade offset respecto al lider al array de offsets
void UFormationComponent::addFormationOffsets(FVector leaderOffset)
{
	formationOffsets.Add(leaderOffset);
}

//Añade un peón a la formación en una determinada posición
void UFormationComponent::addPawnToFormation(APawn* pawn)
{
	pawnsInFormation.AddUnique(pawn);

}

TArray<FVector> UFormationComponent::getFormationOffsets()
{
	return UFormationComponent::formationOffsets;
}

TArray<APawn*> UFormationComponent::getPawnsInFormation()
{
	return UFormationComponent::pawnsInFormation;
}
void UFormationComponent::destroyFormationOffsets()
{
	formationOffsets.Empty();
}

void UFormationComponent::destroyPawnsInFormation()
{
	pawnsInFormation.Empty();
}


// Called when the game starts
void UFormationComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}



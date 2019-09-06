// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FormationComponent.generated.h"

//Struct para almacenar el offset de cada elemento en la formación respecto al líder y al centro.
/*
typedef struct FormationData {

	FVector centerOffset;

	FVector leaderOffset;

} FormationData;
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TFG_API UFormationComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFormationComponent();

	~UFormationComponent();

	//Static functions
	static void addFormationOffsets(FVector leaderOffset);

	static void addPawnToFormation(APawn* pawn);

	UFUNCTION(BlueprintCallable, Category = "Formation Structures")
	static TArray<FVector> getFormationOffsets();

	UFUNCTION(BlueprintCallable, Category = "Formation Structures")
	static TArray<APawn*> getPawnsInFormation();

	static void destroyFormationOffsets();

	static void destroyPawnsInFormation();


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:

private:

	//Orders of each element in the formation. 0 is leader.
	static TArray<APawn*> pawnsInFormation;
	//Offset from the leader of the formation of each element.
	static TArray<FVector> formationOffsets;
};

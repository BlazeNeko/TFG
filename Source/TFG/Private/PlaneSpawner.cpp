// Fill out your copyright notice in the Description page of Project Settings.


#include "PlaneSpawner.h"
#include "PathComponent.h"
#include "FormationComponent.h"
#include "TFGPawn.h"


// Sets default values
APlaneSpawner::APlaneSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlaneSpawner::BeginPlay()
{
	Super::BeginPlay();


	float angle = (float)360 / numPlanes;
	auto path = FindComponentByClass<UPathComponent>()->getPathArray();

	for (int i = 0; i < numPlanes; i++) {
		//Calculate the position of each plane
		FRotator rotation = FRotator(0, i * angle, 0);
		FVector pos = rotation.Vector() * formationRadius;



		//Spawn the planes
		FActorSpawnParameters params = FActorSpawnParameters();
		ATFGPawn* player = GetWorld()->SpawnActor<ATFGPawn>(spawnableBlueprint.Get(), FTransform(GetActorLocation() + pos), params);

		//Adding each spawned unit to the formation
		UFormationComponent::addPawnToFormation(player);
		FVector leaderOffset = pos - FRotator(0, 0, 0).Vector() * formationRadius;
		UFormationComponent::addFormationOffsets(leaderOffset);

		//Initializing each plane's path
		auto actorPath = player->FindComponentByClass<UPathComponent>();
		if (ensure(actorPath)) {
			actorPath->setPathArray(path);
		}
	}

}

// Called every frame
void APlaneSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


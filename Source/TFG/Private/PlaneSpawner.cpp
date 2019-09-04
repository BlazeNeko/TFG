// Fill out your copyright notice in the Description page of Project Settings.


#include "PlaneSpawner.h"
#include "PathComponent.h"
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
		FRotator rotation = FRotator(0, i * angle, 0);
		FVector pos = rotation.Vector() * formationRadius;
		FActorSpawnParameters params = FActorSpawnParameters();
		ATFGPawn* player = GetWorld()->SpawnActor<ATFGPawn>(spawnableBlueprint.Get(), FTransform(GetActorLocation() + pos), params);
		auto actorPath = player->FindComponentByClass<UPathComponent>();
		actorPath->setPathArray(path);
	}

}

// Called every frame
void APlaneSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


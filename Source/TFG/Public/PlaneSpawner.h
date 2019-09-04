// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "PlaneSpawner.generated.h"

UCLASS()
class TFG_API APlaneSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlaneSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Número de aviones a invocar
	UPROPERTY(EditAnywhere, Category = "Spawn Settings")
		int32 numPlanes = 4;
	UPROPERTY(EditAnywhere, Category = "Spawn Settings")
		float formationRadius = 300;
	UPROPERTY(EditAnywhere, Category = "Spawn Settings", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class ATFGPawn> spawnableBlueprint;
};

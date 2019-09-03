// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SensorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TFG_API USensorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USensorComponent();

	//-------------------------------------------------------
	//-------Resultado de la Detección de obstáculos---------
	//-------------------------------------------------------

	//Si los obstáculos están muy cerca, es true
	UPROPERTY(BlueprintReadOnly, Category = "Obstacle Avoidance")
		bool avoid = false;
	//Vector normal del punto de impacto del láser con el obstáculo más cercano
	UPROPERTY(BlueprintReadOnly, Category = "Obstacle Avoidance Result")
		FVector hitNormal;
	//Ubicación en World Coordinates del punto de impacto del láser con el obstáculo más cercano
	UPROPERTY(BlueprintReadOnly, Category = "Obstacle Avoidance Result")
		FVector hitLocation;
	//Sensor que detecta el obstáculo más cercano. H o V (horizontal o vertical)
	UPROPERTY(BlueprintReadOnly, Category = "Obstacle Avoidance Result")
		FString direction = "";
	//1 = arriba o izquierda, -1 = derecha o abajo
	UPROPERTY(BlueprintReadOnly, Category = "Obstacle Avoidance Result")
		int32 way;


protected:																																																																																																																																																																																																																																																																																													
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	//---------------------------
	//-------Debug Params--------
	//---------------------------
	UPROPERTY(EditAnywhere, Category = "Debug")
		bool debug;
	//Lineas de debug persistentes
	UPROPERTY(EditAnywhere, Category = "Debug")
		bool debugPersistentLines = false;
	//Duración de las lineas de debug en caso de ser persistentes
	UPROPERTY(EditAnywhere, Category = "Debug")
		float debugDuration = 0.0f;
	//Grosor de las lineas de debug
	UPROPERTY(EditAnywhere, Category = "Debug")
		float debugLineThickness = 1.5f;
	
	//---------------------------
	//-------Sensor Data---------
	//---------------------------

	//Distancia mínima a la que se permiten los obstáculos antes de esquivar
	UPROPERTY(EditAnywhere, Category = "Laser Settings")
		float threshold = 400;
	//Número de lásers por cada array de lásers
	UPROPERTY(EditAnywhere, Category = "Laser Settings")
		float LasersPerArray = 20;
	//Distancia de los lásers de los sensores horizontales y verticales
	UPROPERTY(EditAnywhere, Category = "Laser Settings")
		float laserDistance = 700;
	/*
	//Distancia del láser caja
	UPROPERTY(EditAnywhere, Category = "Laser Settings")
		float boxRayDistance = 1000;
	//Anchura del láser caja
	UPROPERTY(EditAnywhere, Category = "Laser Settings")
		float boxRayWidth = 100;
	*/


	//Arrays de posiciones de lásers
	TArray<FVector> sensorPositionLR;
	TArray<FVector> sensorPositionUD;
	//Resultados de los lásers
	TArray<FHitResult> sensorH;
	TArray<FHitResult> sensorV;
	//True si el láser ha detectado obstáculo, false si no
	TArray<bool> hitsH;
	TArray<bool> hitsV;

	/*
	FHitResult boxRay;
	bool hitBox = false;
	*/


	//Calcula los lásers.
	//Implementado mediante Vectores y Lerp (interpolación lineal).
	//Funciona independientemente de la rotación del actor. 
	//Tiene la desventaja de que los lásers calculados mediante la interpolación son más cortos al ser lineal.
	void laserSetupLerp(FVector actorLocation);

	//Calcula los lásers
	//Implementación mediante Quaternions y Slerp (interpolación esférica).
	//Funciona sólo si el actor rota exclusivamente sobre el eje Z (yaw rotation). Si rota sobre los ejes X(roll) o Y (pitch), los cálculos con los quaternions no son correctos.
	//Todos los lásers tienen la misma longitud debido a que se calculan mediante la interpolación esférica.
	void laserSetupSlerp(FVector actorLocation);

	//Dibuja las lineas de debug de los lásers ya calculados.
	void debugLines(FVector actorLocation);


	void rayCast(FVector actorLocation);

	void checkRayResult();

};

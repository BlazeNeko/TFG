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
	//Duraci�n de las lineas de debug en caso de ser persistentes
	UPROPERTY(EditAnywhere, Category = "Debug")
		float debugDuration = 0.0f;
	//Grosor de las lineas de debug
	UPROPERTY(EditAnywhere, Category = "Debug")
		float debugLineThickness = 1.5f;
	
	//---------------------------
	//-------Sensor Data---------
	//---------------------------

	//N�mero de l�sers por cada array de l�sers
	UPROPERTY(EditAnywhere, Category = "Laser Settings")
		float LasersPerArray = 20;
	//Distancia de los l�sers de los sensores horizontales y verticales
	UPROPERTY(EditAnywhere, Category = "Laser Settings")
		float laserDistance = 700;
	//Distancia del l�ser caja
	UPROPERTY(EditAnywhere, Category = "Laser Settings")
		float boxRayDistance = 1000;
	//Anchura del l�ser caja
	UPROPERTY(EditAnywhere, Category = "Laser Settings")
		float boxRayWidth = 100;

	//Arrays de posiciones de l�sers
	TArray<FVector> sensorPositionLR;
	TArray<FVector> sensorPositionUD;
	TArray<FHitResult> sensorLR;
	TArray<FHitResult> sensorUD;
	FHitResult boxRay;

	//Calcula los l�sers.
	//Implementado mediante Vectores y Lerp (interpolaci�n lineal).
	//Funciona independientemente de la rotaci�n del actor. 
	//Tiene la desventaja de que los l�sers calculados mediante la interpolaci�n son m�s cortos al ser lineal.
	void laserSetupLerp(FVector actorLocation);

	//Calcula los l�sers
	//Implementaci�n mediante Quaternions y Slerp (interpolaci�n esf�rica).
	//Funciona s�lo si el actor rota exclusivamente sobre el eje Z (yaw rotation). Si rota sobre los ejes X(roll) o Y (pitch), los c�lculos con los quaternions no son correctos.
	//Todos los l�sers tienen la misma longitud debido a que se calculan mediante la interpolaci�n esf�rica.
	void laserSetupSlerp(FVector actorLocation);

	//Dibuja las lineas de debug de los l�sers ya calculados.
	void debugLines(FVector actorLocation);

	void rayCast(FVector actorLocation);
};

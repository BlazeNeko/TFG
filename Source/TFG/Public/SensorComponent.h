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
	//-------Resultado de la Detecci�n de obst�culos---------
	//-------------------------------------------------------
	//Sensores horizontal y vertical

	//Si los obst�culos est�n muy cerca, es true

	//Vector normal del punto de impacto del l�ser con el obst�culo m�s cercano
	UPROPERTY(BlueprintReadOnly, Category = "Obstacle Avoidance Result")
		FVector hitNormal;
	//Ubicaci�n en World Coordinates del punto de impacto del l�ser con el obst�culo m�s cercano
	UPROPERTY(BlueprintReadOnly, Category = "Obstacle Avoidance Result")
		FVector hitLocation;
	//Distancia desde el pawn hasta el punto de impacto
	UPROPERTY(BlueprintReadOnly, Category = "Obstacle Avoidance Result")
		float distance;
	//Sensor que detecta el obst�culo m�s cercano. H o V (horizontal o vertical)
	UPROPERTY(BlueprintReadOnly, Category = "Obstacle Avoidance Result")
		FString direction = "";
	//1 = arriba o izquierda, -1 = derecha o abajo
	UPROPERTY(BlueprintReadOnly, Category = "Obstacle Avoidance Result")
		int32 way;

	//Datos laser caja
	//Si los obst�culos est�n muy cerca, es true
	UPROPERTY(BlueprintReadOnly, Category = "Obstacle Avoidance Result")
		bool avoid;
	//Vector normal del punto de impacto del l�ser con el obst�culo m�s cercano
	UPROPERTY(BlueprintReadOnly, Category = "Obstacle Avoidance Result")
		FVector frontHitNormal;
	//Ubicaci�n en World Coordinates del punto de impacto del l�ser con el obst�culo m�s cercano
	UPROPERTY(BlueprintReadOnly, Category = "Obstacle Avoidance Result")
		FVector frontHitLocation;
	//Distancia desde el pawn hasta el punto de impacto
	UPROPERTY(BlueprintReadOnly, Category = "Obstacle Avoidance Result")
		float frontObstacleDistance;
	//Actor golpeado por el rayo frontal
	UPROPERTY(BlueprintReadOnly, Category = "Obstacle Avoidance Result")
		AActor* boxHitActor;

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

	//Distancia m�nima a la que se permiten los obst�culos antes de esquivar
	UPROPERTY(EditAnywhere, Category = "Laser Settings")
		float minDistance = 400;
	UPROPERTY(EditAnywhere, Category = "Laser Settings")
		float threshold = 600;
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
	//Resultados de los l�sers
	TArray<FHitResult> sensorH;
	TArray<FHitResult> sensorV;
	//True si el l�ser ha detectado obst�culo, false si no
	TArray<bool> hitsH;
	TArray<bool> hitsV;

	//True si el l�ser caja ha detectado obst�culo, false si no
	FHitResult boxRayHit;
	bool boxRayBool = false;
	


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

	void checkRayResult();

};

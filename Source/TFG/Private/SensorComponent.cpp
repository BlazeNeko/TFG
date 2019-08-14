// Fill out your copyright notice in the Description page of Project Settings.


#include "SensorComponent.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Public/DrawDebugHelpers.h"

#include "Runtime/Engine/Classes/Engine/World.h"


// Sets default values for this component's properties
USensorComponent::USensorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USensorComponent::BeginPlay()
{
	Super::BeginPlay();

	sensorPositionLR.Init(FVector(0.0f, 0.0f, 0.0f), LasersPerArray);
	sensorPositionUD.Init(FVector(0.0f, 0.0f, 0.0f), LasersPerArray);
	sensorLR.Init(FHitResult(), LasersPerArray);
	sensorUD.Init(FHitResult(), LasersPerArray);

}


// Called every frame
void USensorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Vector de posición del actor que posee al componente.
	FVector actorLocation = GetOwner()->GetTransform().GetLocation();

	//Inicialización de los sensores
	laserSetupLerp(actorLocation);
	// Debug Lines
	if(debug)
		debugLines(actorLocation);
	//Raycast
	rayCast(actorLocation);
	//FHitResult Hit;  //Parámetro de salida

	//GetWorld()->LineTraceSingleByObjectType(Hit, startPoint, endPointLeftRight);

	

}

//Calcula los lásers.
//Implementado mediante Vectores y Lerp (interpolación lineal).
//Funciona independientemente de la rotación del actor. 
void USensorComponent::laserSetupLerp(FVector actorLocation) {

	//Vector normalizado hacia la dirección a la que mira el actor
	FVector actorForwardVector = GetOwner()->GetActorForwardVector();
	//Vector normalizado hacia la derecha respecto a la dirección a la que mira el actor
	FVector actorLeftVector = -GetOwner()->GetActorRightVector();
	//Vector normalizado hacia la izquierda respecto a la dirección a la que mira el actor
	FVector actorRightVector = GetOwner()->GetActorRightVector();
	//Vector normalizado hacia la derecha respecto a la dirección a la que mira el actor
	FVector actorUpVector = GetOwner()->GetActorUpVector();
	//Vector normalizado hacia la izquierda respecto a la dirección a la que mira el actor
	FVector actorDownVector = -GetOwner()->GetActorUpVector();


	float spaceBetweenLasers = 180.0f / (LasersPerArray - 1.f);
	FVector laserTargetLR, laserTargetUD;
	for (int32 i = 0; i < LasersPerArray; i++) {
		//Cálculo de alpha.
		float alpha = FMath::Clamp((i * spaceBetweenLasers / 180.f * 2), 0.f, 2.f);

		if (alpha <= 1) {
			//Efectúa una interpolación lineal entre los vectores "izquierda" y "delante" relativos al actor, en base a alpha
			laserTargetLR = FMath::Lerp(actorLeftVector, actorForwardVector, alpha);
			//Efectúa una interpolación lineal entre los vectores "arriba" y "delante" relativos al actor, en base a alpha
			laserTargetUD = FMath::Lerp(actorUpVector, actorForwardVector, alpha);
		}
		else {
			//Efectúa una interpolación lineal entre los vectores "delante" y "derecha" relativos al actor, en base a alpha
			laserTargetLR = FMath::Lerp(actorForwardVector, actorRightVector, alpha - 1);
			//Efectúa una interpolación lineal entre los vectores "delante" y "abajo" relativos al actor, en base a alpha
			laserTargetUD = FMath::Lerp(actorForwardVector, actorDownVector, alpha - 1);
		}
		sensorPositionLR[i] = actorLocation + laserTargetLR * laserDistance * laserDistance /(laserTargetLR * laserDistance).Size();
		sensorPositionUD[i] = actorLocation + laserTargetUD * laserDistance * laserDistance /(laserTargetUD * laserDistance).Size();
		//UE_LOG(LogTemp, Warning, TEXT("laser target %n: %s"), i, *laserTarget.ToString())
	}
}

//Calcula los lásers
//Implementación mediante Quaternions y Slerp (interpolación esférica).
//Funciona sólo si el actor rota exclusivamente sobre el eje Z (yaw rotation). Si rota sobre los ejes X(roll) o Y (pitch), los cálculos con los quaternions no son correctos.
//Todos los lásers tienen la misma longitud debido a que se calculan mediante la interpolación esférica.
void USensorComponent::laserSetupSlerp(FVector actorLocation)
{

	//Quaternion normalizado hacia la dirección a la que mira el actor
	FQuat actorForwardQuaternion = GetOwner()->GetActorForwardVector().ToOrientationQuat();
	//Quaternions normalizados hacia la izquierda y derecha respecto a la dirección a la que mira el actor
	FQuat actorLeftQuaternion = (-GetOwner()->GetActorRightVector()).ToOrientationQuat();
	//Quaternion normalizado hacia la derecha respecto a la dirección a la que mira el actor
	FQuat actorRightQuaternion = GetOwner()->GetActorRightVector().ToOrientationQuat();
	//Quaternion normalizado hacia arriba respecto a la dirección a la que mira el actor
	FQuat actorUpQuaternion = GetOwner()->GetActorUpVector().ToOrientationQuat();
	//Quaternion normalizado hacia abajo respecto a la dirección a la que mira el actor
	FQuat actorDownQuaternion = (-GetOwner()->GetActorUpVector()).ToOrientationQuat();

	FVector laserTargetLR, laserTargetUD;

	float spaceBetweenLasers = 180.0f / (LasersPerArray - 1.f);
	for (int32 i = 0; i < LasersPerArray; i++) {
		//Cálculo de alpha.
		float alpha = FMath::Clamp((i * spaceBetweenLasers / 90.f), 0.f, 2.f);

		if (alpha <= 1) {
			//Efectúa una interpolación esférica entre los quaternions "izquierda" y "delante" relativos al actor, en base a alpha
			laserTargetLR = FQuat::Slerp(actorLeftQuaternion, actorForwardQuaternion, alpha).Vector();
			//Efectúa una interpolación esférica entre los quaternions "Arriba" y "delante" relativos al actor, en base a alpha
			laserTargetUD = FQuat::Slerp(actorUpQuaternion, actorForwardQuaternion, alpha).Vector();
		}
		else {
			//Efectúa una interpolación esférica entre los quaternions "delante" y "derecha" relativos al actor, en base a alpha
			laserTargetLR = FQuat::Slerp(actorForwardQuaternion, actorRightQuaternion, alpha - 1).Vector();
			//Efectúa una interpolación esférica entre los quaternions "delante" y "Abajo" relativos al actor, en base a alpha
			laserTargetUD = FQuat::Slerp(actorForwardQuaternion, actorDownQuaternion, alpha - 1).Vector();
		}
		sensorPositionLR[i] = actorLocation + laserTargetLR * laserDistance;
		sensorPositionUD[i] = actorLocation + laserTargetUD * laserDistance;
	}
}

//Dibuja las lineas de debug de los lásers ya calculados.
void USensorComponent::debugLines(FVector actorLocation) {
	
	if (sensorPositionLR.IsValidIndex(0)) {

		for (auto item : sensorPositionLR) {
			DrawDebugLine(GetWorld(), actorLocation, item, FColor(135, 0, 145), debugPersistentLines, debugDuration, 0.f, debugLineThickness);
		}

	} else {
		UE_LOG(LogTemp, Error, TEXT("Posiciones de los lásers del sensor LeftRight no inicializadas"))
	}

	if(sensorPositionUD.IsValidIndex(0)){

		for (auto item : sensorPositionUD) {
			DrawDebugLine(GetWorld(), actorLocation, item, FColor(135, 0, 145), debugPersistentLines, debugDuration, 0.f, debugLineThickness);
		}

	} else {
		UE_LOG(LogTemp, Error, TEXT("Posiciones de los lásers del sensor UpDown no inicializadas"))

	}
	
	DrawDebugBox(
		GetWorld(),
		actorLocation + GetOwner()->GetActorForwardVector() * boxRayDistance / 2.f,
		FVector(boxRayDistance/2.f, boxRayWidth/2.f, boxRayWidth/2.f),
		GetOwner()->GetActorForwardVector().ToOrientationQuat(),
		FColor(0, 255, 0), debugPersistentLines,
		debugDuration,
		0,
		debugLineThickness);
	//		GetOwner()->GetActorForwardVector() * boxRayDistance/2.f,

}

void USensorComponent::rayCast(FVector actorLocation) {

	//Inicialización de la forma del box ray
	FCollisionShape shape = FCollisionShape::MakeBox(FVector(boxRayWidth, boxRayWidth, boxRayWidth));

	GetWorld()->SweepSingleByObjectType(
		boxRay,																//FHitResult
		actorLocation,														//start position of the box ray, FVector
		GetOwner()->GetActorForwardVector() * boxRayDistance,				//end position of the box ray, FVector
		GetOwner()->GetActorForwardVector().ToOrientationQuat(),			//Rotation of the box ray, FQuat
		FCollisionObjectQueryParams(ECollisionChannel::ECC_WorldStatic),    //Filtro de resultados para los rayos (sólo objetos estáticos)
		shape
	);
	
	//Inicialización de los parámetros del RayCast para los sensores de rayos lineales
	FCollisionQueryParams params(
		FName(TEXT("")),				//trace tag
		false,							//false == trazas simples (simplifica los mesh con los que impactan los rayos). True = trazas complejas (no simplifica los mesh)
		GetOwner()						//Objetos a ignorar (GetOwner para que los rayos no impacten con uno mismo)
	);

	for (int32 i = 0; i < LasersPerArray; i++) {
		GetWorld()->LineTraceSingleByObjectType(
			sensorLR[i],														//FHitResult
			actorLocation,														//start position of the ray, FVector
			sensorPositionLR[i],												//end position of the ray, FVector
			FCollisionObjectQueryParams(ECollisionChannel::ECC_WorldStatic),    //Filtro de resultados para los rayos (sólo objetos estáticos)
			params																//parámetros definidos previamente
		);
		GetWorld()->LineTraceSingleByObjectType(								//FHitResult
			sensorUD[i],														//start position of the ray, FVector
			actorLocation,														//end position of the ray, FVector
			sensorPositionUD[i],												//Filtro de resultados para los rayos (sólo objetos estáticos)
			FCollisionObjectQueryParams(ECollisionChannel::ECC_WorldStatic),	//parámetros definidos previamente
			params
		);

		if (debug) {
			AActor* obstacle = sensorLR[i].GetActor();
			AActor* obstacle2 = sensorUD[i].GetActor();
			if (obstacle) {
				UE_LOG(LogTemp, Warning, TEXT("LaserLR %d hit: %s"), i, *(obstacle->GetName()))
			}
			if (obstacle2) {
				UE_LOG(LogTemp, Warning, TEXT("LaserUD %d hit: %s"), i, *(obstacle2->GetName()))
			}
		}
	}

}
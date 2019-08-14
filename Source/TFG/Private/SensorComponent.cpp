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
	sensorH.Init(FHitResult(), LasersPerArray);
	sensorV.Init(FHitResult(), LasersPerArray);

}


// Called every frame
void USensorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Vector de posici�n del actor que posee al componente.
	FVector actorLocation = GetOwner()->GetTransform().GetLocation();

	//Inicializaci�n de los sensores
	laserSetupLerp(actorLocation);
	// Debug Lines
	if(debug)
		debugLines(actorLocation);
	//Raycast
	rayCast(actorLocation);
	//FHitResult Hit;  //Par�metro de salida

	//GetWorld()->LineTraceSingleByObjectType(Hit, startPoint, endPointLeftRight);

	

}

//Calcula los l�sers.
//Implementado mediante Vectores y Lerp (interpolaci�n lineal).
//Funciona independientemente de la rotaci�n del actor. 
void USensorComponent::laserSetupLerp(FVector actorLocation) {

	//Vector normalizado hacia la direcci�n a la que mira el actor
	FVector actorForwardVector = GetOwner()->GetActorForwardVector();
	//Vector normalizado hacia la derecha respecto a la direcci�n a la que mira el actor
	FVector actorLeftVector = -GetOwner()->GetActorRightVector();
	//Vector normalizado hacia la izquierda respecto a la direcci�n a la que mira el actor
	FVector actorRightVector = GetOwner()->GetActorRightVector();
	//Vector normalizado hacia la derecha respecto a la direcci�n a la que mira el actor
	FVector actorUpVector = GetOwner()->GetActorUpVector();
	//Vector normalizado hacia la izquierda respecto a la direcci�n a la que mira el actor
	FVector actorDownVector = -GetOwner()->GetActorUpVector();


	float spaceBetweenLasers = 180.0f / (LasersPerArray - 1.f);
	FVector laserTargetLR, laserTargetUD;
	for (int32 i = 0; i < LasersPerArray; i++) {
		//C�lculo de alpha.
		float alpha = FMath::Clamp((i * spaceBetweenLasers / 180.f * 2), 0.f, 2.f);

		if (alpha <= 1) {
			//Efect�a una interpolaci�n lineal entre los vectores "izquierda" y "delante" relativos al actor, en base a alpha
			laserTargetLR = FMath::Lerp(actorLeftVector, actorForwardVector, alpha);
			//Efect�a una interpolaci�n lineal entre los vectores "arriba" y "delante" relativos al actor, en base a alpha
			laserTargetUD = FMath::Lerp(actorUpVector, actorForwardVector, alpha);
		}
		else {
			//Efect�a una interpolaci�n lineal entre los vectores "delante" y "derecha" relativos al actor, en base a alpha
			laserTargetLR = FMath::Lerp(actorForwardVector, actorRightVector, alpha - 1);
			//Efect�a una interpolaci�n lineal entre los vectores "delante" y "abajo" relativos al actor, en base a alpha
			laserTargetUD = FMath::Lerp(actorForwardVector, actorDownVector, alpha - 1);
		}
		sensorPositionLR[i] = actorLocation + laserTargetLR * laserDistance * laserDistance /(laserTargetLR * laserDistance).Size();
		sensorPositionUD[i] = actorLocation + laserTargetUD * laserDistance * laserDistance /(laserTargetUD * laserDistance).Size();
		//UE_LOG(LogTemp, Warning, TEXT("laser target %n: %s"), i, *laserTarget.ToString())
	}
}

//Calcula los l�sers
//Implementaci�n mediante Quaternions y Slerp (interpolaci�n esf�rica).
//Funciona s�lo si el actor rota exclusivamente sobre el eje Z (yaw rotation). Si rota sobre los ejes X(roll) o Y (pitch), los c�lculos con los quaternions no son correctos.
//Todos los l�sers tienen la misma longitud debido a que se calculan mediante la interpolaci�n esf�rica.
void USensorComponent::laserSetupSlerp(FVector actorLocation)
{

	//Quaternion normalizado hacia la direcci�n a la que mira el actor
	FQuat actorForwardQuaternion = GetOwner()->GetActorForwardVector().ToOrientationQuat();
	//Quaternions normalizados hacia la izquierda y derecha respecto a la direcci�n a la que mira el actor
	FQuat actorLeftQuaternion = (-GetOwner()->GetActorRightVector()).ToOrientationQuat();
	//Quaternion normalizado hacia la derecha respecto a la direcci�n a la que mira el actor
	FQuat actorRightQuaternion = GetOwner()->GetActorRightVector().ToOrientationQuat();
	//Quaternion normalizado hacia arriba respecto a la direcci�n a la que mira el actor
	FQuat actorUpQuaternion = GetOwner()->GetActorUpVector().ToOrientationQuat();
	//Quaternion normalizado hacia abajo respecto a la direcci�n a la que mira el actor
	FQuat actorDownQuaternion = (-GetOwner()->GetActorUpVector()).ToOrientationQuat();

	FVector laserTargetLR, laserTargetUD;

	float spaceBetweenLasers = 180.0f / (LasersPerArray - 1.f);
	for (int32 i = 0; i < LasersPerArray; i++) {
		//C�lculo de alpha.
		float alpha = FMath::Clamp((i * spaceBetweenLasers / 90.f), 0.f, 2.f);

		if (alpha <= 1) {
			//Efect�a una interpolaci�n esf�rica entre los quaternions "izquierda" y "delante" relativos al actor, en base a alpha
			laserTargetLR = FQuat::Slerp(actorLeftQuaternion, actorForwardQuaternion, alpha).Vector();
			//Efect�a una interpolaci�n esf�rica entre los quaternions "Arriba" y "delante" relativos al actor, en base a alpha
			laserTargetUD = FQuat::Slerp(actorUpQuaternion, actorForwardQuaternion, alpha).Vector();
		}
		else {
			//Efect�a una interpolaci�n esf�rica entre los quaternions "delante" y "derecha" relativos al actor, en base a alpha
			laserTargetLR = FQuat::Slerp(actorForwardQuaternion, actorRightQuaternion, alpha - 1).Vector();
			//Efect�a una interpolaci�n esf�rica entre los quaternions "delante" y "Abajo" relativos al actor, en base a alpha
			laserTargetUD = FQuat::Slerp(actorForwardQuaternion, actorDownQuaternion, alpha - 1).Vector();
		}
		sensorPositionLR[i] = actorLocation + laserTargetLR * laserDistance;
		sensorPositionUD[i] = actorLocation + laserTargetUD * laserDistance;
	}
}


void USensorComponent::rayCast(FVector actorLocation) {

	//Inicializaci�n de la forma del box ray
	FCollisionShape shape = FCollisionShape::MakeBox(FVector(boxRayWidth, boxRayWidth, boxRayWidth));

	GetWorld()->SweepSingleByObjectType(
		boxRay,																//FHitResult
		actorLocation,														//start position of the box ray, FVector
		actorLocation + GetOwner()->GetActorForwardVector() * boxRayDistance,				//end position of the box ray, FVector
		GetOwner()->GetTransform().GetRotation(),			//Rotation of the box ray, FQuat
		FCollisionObjectQueryParams(ECollisionChannel::ECC_WorldStatic),    //Filtro de resultados para los rayos (s�lo objetos est�ticos)
		shape
	);
	
	//Inicializaci�n de los par�metros del RayCast para los sensores de rayos lineales
	FCollisionQueryParams params(
		FName(TEXT("")),				//trace tag
		false,							//false == trazas simples (simplifica los mesh con los que impactan los rayos). True = trazas complejas (no simplifica los mesh)
		GetOwner()						//Objetos a ignorar (GetOwner para que los rayos no impacten con uno mismo)
	);

	for (int32 i = 0; i < LasersPerArray; i++) {
		GetWorld()->LineTraceSingleByObjectType(
			sensorH[i],														//FHitResult
			actorLocation,														//start position of the ray, FVector
			sensorPositionLR[i],												//end position of the ray, FVector
			FCollisionObjectQueryParams(ECollisionChannel::ECC_WorldStatic),    //Filtro de resultados para los rayos (s�lo objetos est�ticos)
			params																//par�metros definidos previamente
		);
		GetWorld()->LineTraceSingleByObjectType(								//FHitResult
			sensorV[i],														//start position of the ray, FVector
			actorLocation,														//end position of the ray, FVector
			sensorPositionUD[i],												//Filtro de resultados para los rayos (s�lo objetos est�ticos)
			FCollisionObjectQueryParams(ECollisionChannel::ECC_WorldStatic),	//par�metros definidos previamente
			params
		);

		if (debug) {
			AActor* obstacle = sensorH[i].GetActor();
			AActor* obstacle2 = sensorV[i].GetActor();
			if (obstacle) {
				UE_LOG(LogTemp, Warning, TEXT("LaserLR %d hit: %s"), i, *(obstacle->GetName()))
			}
			if (obstacle2) {
				UE_LOG(LogTemp, Warning, TEXT("LaserUD %d hit: %s"), i, *(obstacle2->GetName()))
			}
		}
	}

}

//Dibuja las lineas de debug de los l�sers ya calculados.
void USensorComponent::debugLines(FVector actorLocation) {

	//Dibujar box ray
	DrawDebugBox(
		GetWorld(),																				//Mundo
		actorLocation + GetOwner()->GetActorForwardVector() * boxRayDistance / 2.f,				//Centro de la caja
		FVector(boxRayDistance / 2.f, boxRayWidth / 2.f, boxRayWidth / 2.f),							//extensi�n de la caja desde el centro a cada direcci�n
		GetOwner()->GetTransform().GetRotation(),								//Rotaci�n de la caja
		FColor(0, 255, 0),																		//Color de la caja
		debugPersistentLines,																	//L�neas persistentes (false por defecto)							
		debugDuration,																			//Duraci�n de las l�neas de la caja
		0,
		debugLineThickness);																	//Grosor de las l�neas de debug

	//Output box ray
	AActor* boxHit = boxRay.GetActor();
	if (boxHit) {
		UE_LOG(LogTemp, Warning, TEXT("Laser box hit: %s"), *(boxHit->GetName()))
		DrawDebugLine(GetWorld(), actorLocation, boxRay.Location, FColor::Blue, debugPersistentLines, 1.f, 0.f, debugLineThickness);

	}

	//Dibujar sensor horizontal
	if (sensorPositionLR.IsValidIndex(0)) {

		for (auto item : sensorPositionLR) {
			DrawDebugLine(GetWorld(), actorLocation, item, FColor(135, 0, 145), debugPersistentLines, debugDuration, 0.f, debugLineThickness);
		}

	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Posiciones de los l�sers del sensor horizontal no inicializadas"))
	}

	//Dibujar sensor vertical
	if (sensorPositionUD.IsValidIndex(0)) {

		for (auto item : sensorPositionUD) {
			DrawDebugLine(GetWorld(), actorLocation, item, FColor(135, 0, 145), debugPersistentLines, debugDuration, 0.f, debugLineThickness);
		}

	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Posiciones de los l�sers del sensor vertical no inicializadas"))

	}

	//Output sensores horizontal y vertical
	for (int32 i = 0; i < LasersPerArray; i++) {

		AActor* obstacleH = sensorH[i].GetActor();
		AActor* obstacleV = sensorV[i].GetActor();

		if (obstacleH) {
			UE_LOG(LogTemp, Warning, TEXT("LaserH %d hit: %s"), i, *(obstacleH->GetName()))
			DrawDebugLine(GetWorld(), actorLocation, sensorH[i].Location, FColor::Black, debugPersistentLines, 1.f, 0.f, debugLineThickness);
		}
		if (obstacleV) {
			UE_LOG(LogTemp, Warning, TEXT("LaserV %d hit: %s"), i, *(obstacleV->GetName()))
			DrawDebugLine(GetWorld(), actorLocation, sensorV[i].Location, FColor::White, debugPersistentLines, 1.f, 0.f, debugLineThickness);
		}
	}
}

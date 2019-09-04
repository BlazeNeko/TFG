// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "PathComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TFG_API UPathComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	TArray<AActor*> getPathArray() const;

	void setPathArray(TArray<AActor*> path);
private:
	UPROPERTY(EditInstanceOnly, Category = "Path")
	TArray<AActor*> pathArray;
			
};

// Fill out your copyright notice in the Description page of Project Settings.

#include "PathComponent.h"
	
TArray<AActor*> UPathComponent::getPathArray() const{
	return pathArray;
}
void UPathComponent::setPathArray(TArray<AActor*> path) {
	pathArray = path;
}

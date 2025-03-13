// Fill out your copyright notice in the Description page of Project Settings.


#include "ObstacleComponent.h"
#include "Kismet/KismetSystemLibrary.h"

UObstacleComponent::UObstacleComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UObstacleComponent::NotifyCompleteJump()
{
	UE_LOG(LogTemp, Log, TEXT("Obstacle Jumped"))
}


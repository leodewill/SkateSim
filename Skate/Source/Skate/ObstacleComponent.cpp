// Fill out your copyright notice in the Description page of Project Settings.


#include "ObstacleComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PointSubsystem.h"

UObstacleComponent::UObstacleComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UObstacleComponent::NotifyCompleteJump()
{
	GetWorld()->GetGameInstance()->GetSubsystem<UPointSubsystem>()->Score(ScoreReward);
	OnScored.Broadcast(ScoreReward);
}


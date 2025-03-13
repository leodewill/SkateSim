// Fill out your copyright notice in the Description page of Project Settings.


#include "SkateHUD.h"
#include <Skate/PointSubsystem.h>

void ASkateHUD::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetGameInstance()->GetSubsystem<UPointSubsystem>()->OnScoreChanged.AddUniqueDynamic(this, &ASkateHUD::OnScoreChanged);
}

void ASkateHUD::OnScoreChanged(int Value, const FScoringData& Data)
{
	K2_OnScoreChanged(Value, Data);
}

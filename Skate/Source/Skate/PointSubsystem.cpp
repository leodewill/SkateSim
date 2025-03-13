// Fill out your copyright notice in the Description page of Project Settings.


#include "PointSubsystem.h"

void UPointSubsystem::Score(const FScoringData& Points)
{
	CurrentScore += Points.ScoreModifier;
	OnScoreChanged.Broadcast(CurrentScore, Points);
}

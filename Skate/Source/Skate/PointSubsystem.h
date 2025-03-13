// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SkateImportantTypes.h"
#include "PointSubsystem.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPointScoreSignature, int, Value, const FScoringData&, Data);

UCLASS()
class SKATE_API UPointSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	void Score(const FScoringData& Points);

	FPointScoreSignature OnScoreChanged;

private:
	int CurrentScore;
};

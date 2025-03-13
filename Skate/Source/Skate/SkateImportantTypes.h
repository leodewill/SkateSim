// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkateImportantTypes.generated.h"

USTRUCT(BlueprintType)
struct FScoringData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Message;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ScoreModifier;

public:
	FScoringData() : Message(FText::FromString("Obstacle Jumped!")), ScoreModifier(0) {}
};


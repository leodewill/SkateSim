// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SkateImportantTypes.h"
#include "ObstacleComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FObstacleScoreSignature, const FScoringData&, Score);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SKATE_API UObstacleComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UObstacleComponent();

	void NotifyCompleteJump();

protected:
	UPROPERTY(BlueprintAssignable)
	FObstacleScoreSignature OnScored;

	UPROPERTY(EditDefaultsOnly, Category = "Points")
	FScoringData ScoreReward;
};

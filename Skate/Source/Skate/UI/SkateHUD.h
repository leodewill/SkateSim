// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SkateHUD.generated.h"

/**
 * 
 */
UCLASS()
class SKATE_API ASkateHUD : public AHUD
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent, DisplayName = "OnScoreChanged")
	void K2_OnScoreChanged(int Value, const FScoringData& Data);

private:
	UFUNCTION()
	void OnScoreChanged(int Value, const FScoringData& Data);
};

// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SkateGameMode.generated.h"

UCLASS(minimalapi)
class ASkateGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASkateGameMode();

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

private:
	UFUNCTION()
	void OnPlayerScored(int Value, const FScoringData& Data);

protected:
	UFUNCTION(BlueprintImplementableEvent, DisplayName="OnPlayerScored")
	void K2_OnPlayerScored(int Value, const FScoringData& Data);
};




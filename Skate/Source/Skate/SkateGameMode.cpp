// Copyright Epic Games, Inc. All Rights Reserved.

#include "SkateGameMode.h"
#include "SkateCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "PointSubsystem.h"

ASkateGameMode::ASkateGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void ASkateGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	GetWorld()->GetGameInstance()->GetSubsystem<UPointSubsystem>()->OnScoreChanged.AddUniqueDynamic(this, &ASkateGameMode::OnPlayerScored);
}

void ASkateGameMode::OnPlayerScored(int Value, const FScoringData& Data)
{
	K2_OnPlayerScored(Value, Data);
}

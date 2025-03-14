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

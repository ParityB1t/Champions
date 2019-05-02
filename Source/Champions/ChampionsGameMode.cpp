// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "ChampionsGameMode.h"
#include "PlayerCharacter.h"

AChampionsGameMode::AChampionsGameMode()
{
	// set default pawn class to our character class
	DefaultPawnClass = APlayerCharacter::StaticClass();

    /*AGoblinController* controller = GetWorld()->SpawnActor<AGoblinController>(GetActorLocation(), GetActorRotation());
    controller->Possess(this);*/
}


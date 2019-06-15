// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "TetrisGameMode.h"
#include "TetrisPlayerController.h"
#include "TetrisPawn.h"

ATetrisGameMode::ATetrisGameMode()
{
	// no pawn by default
	DefaultPawnClass = ATetrisPawn::StaticClass();
	// use our own player controller class
	PlayerControllerClass = ATetrisPlayerController::StaticClass();
}

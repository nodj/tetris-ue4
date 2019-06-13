// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Tetris_GameGameMode.h"
#include "Tetris_GamePlayerController.h"
#include "Tetris_GamePawn.h"

ATetris_GameGameMode::ATetris_GameGameMode()
{
	// no pawn by default
	DefaultPawnClass = ATetris_GamePawn::StaticClass();
	// use our own player controller class
	PlayerControllerClass = ATetris_GamePlayerController::StaticClass();
}

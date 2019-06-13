// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Tetris_GamePlayerController.h"

ATetris_GamePlayerController::ATetris_GamePlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableTouchEvents = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

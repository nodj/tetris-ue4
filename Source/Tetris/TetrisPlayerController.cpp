// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "TetrisPlayerController.h"

ATetrisPlayerController::ATetrisPlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableTouchEvents = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

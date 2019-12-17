// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "TetrisBlockGrid.h"

#include "TetrisBlock.h"
#include "TetrisGameInstance.h"

#include "core/modes/GameModeStandard.h"

#include "Components/TextRenderComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/World.h"

#if WITH_EDITOR
#include "DrawDebugHelpers.h"
#include "Editor/EditorEngine.h"
#include "Editor/UnrealEdEngine.h"
#include "UnrealEdGlobals.h"
#endif


#define LOCTEXT_NAMESPACE "PuzzleBlockGrid"


ATetrisBlockGrid::ATetrisBlockGrid()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("root"));
}

void ATetrisBlockGrid::BeginPlay()
{
	Super::BeginPlay();

	if (UTetrisGameInstance* GI = GetGameInstance<UTetrisGameInstance>())
	{
		GI->TheGrid = this;
	}

	StdGameMode = Tetris.SetupStandardMode(Width, Height);
	tc::StandardGameMode& Game = *StdGameMode;
	const tc::Board& GameBoard = Game.GetBoard();
	Width = GameBoard.GetWidth();
	Height = GameBoard.GetHeight();
	UWorld& World = *GetWorld();
	const float CenterOffset = -0.5f * (Width-1) * BlockSpacing;
	for (int32 BlockI = 0; BlockI < Width; ++BlockI)
	{
		for (int32 BlockJ = 0; BlockJ < Height; ++BlockJ)
		{
			const float YOffset = CenterOffset + BlockI * BlockSpacing;
			const float XOffset = BlockJ * BlockSpacing;

			// Make position vector, offset from Grid location
			const FVector BlockLocation = FVector(XOffset, YOffset, 0.f) + GetActorLocation();

			// Spawn a block
			if (ATetrisBlock* NewBlock = World.SpawnActor<ATetrisBlock>(BlockLocation, FRotator(0,0,0)))
			{
				NewBlock->SetActorRelativeScale3D(BlockScaleFactor * NewBlock->GetActorRelativeScale3D());
				NewBlock->SetCell(&GameBoard.At(BlockI, BlockJ));
				// Tell the block about its owner
				NewBlock->OwningGrid = this;
			}
		}
	}
}

void ATetrisBlockGrid::Tick(float DeltaSeconds)
{
	if (StdGameMode)
	{
		StdGameMode->Tick(DeltaSeconds);

		// Update stats
		if (StdGameMode->IsInPlayMode())
		{
			const tc::BasicGameStats& InternalStats = StdGameMode->GetBasicStats();
			Stats.LineCount = InternalStats.Lines;
			Stats.Level = InternalStats.Level;
			Stats.Score = InternalStats.Score;
		}
	}

	// Game exit
	if (StdGameMode == nullptr || StdGameMode->IsOver())
	{
#if WITH_EDITOR
		// RequestExit() ends the game loop. Including the Editor
		GUnrealEd->RequestEndPlayMap();
#else
		FGenericPlatformMisc::RequestExit(false);
#endif
	}

#if WITH_EDITOR
	if (GIsPlayInEditorWorld)
	{
		return;
	}

	const float CenterOffset = -0.5f * (Width - 1) * BlockSpacing;
	for (int32 BlockI = 0; BlockI < Width; ++BlockI)
	{
		for (int32 BlockJ = 0; BlockJ < Height; ++BlockJ)
		{
			const float YOffset = CenterOffset + BlockI * BlockSpacing;
			const float XOffset = BlockJ * BlockSpacing;

			// Make position vector, offset from Grid location
			const FVector BlockLocation = FVector(XOffset, YOffset, 0.f) + GetActorLocation();
			DrawDebugBox(GetWorld(), BlockLocation, FVector(40.0f), FColor::Cyan);
		}
	}
#endif
}

void ATetrisBlockGrid::HandleInput(tc::EGameplayInput i)
{
	Tetris.GetCurrentMode()->RegisterInput(i);
}

#undef LOCTEXT_NAMESPACE

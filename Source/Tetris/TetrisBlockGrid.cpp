// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "TetrisBlockGrid.h"

#include "TetrisBlock.h"
#include "TetrisGameInstance.h"

#include "core/modes/GameModeStandard.h"

#include "Components/TextRenderComponent.h"
#include "ConstructorHelpers.h"
#include "Engine/World.h"
#if WITH_EDITOR
#include "DrawDebugHelpers.h"
#endif


#define LOCTEXT_NAMESPACE "PuzzleBlockGrid"


ATetrisBlockGrid::ATetrisBlockGrid()
{
#if WITH_EDITOR
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
// 	PrimaryActorTick.bTickEvenWhenPaused = true;
#endif

	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;

	// Create static mesh component
	ScoreText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("ScoreText0"));
	ScoreText->SetRelativeLocation(FVector(-200.f,-200.f,0.f));
	ScoreText->SetRelativeRotation(FRotator(90.f, 180.f, 0.f));
	ScoreText->SetRelativeScale3D(FVector(5.0f));
	ScoreText->SetupAttachment(DummyRoot);
	UpdateText();
}

void ATetrisBlockGrid::BeginPlay()
{
	Super::BeginPlay();

	if (UTetrisGameInstance* GI = GetGameInstance<UTetrisGameInstance>())
	{
		GI->TheGrid = this;
	}

	const tc::StandardGameMode& Game = Tetris.SetupStandardMode(Width, Height);
	const tc::Board& GameBoard = Game.GetBoard();
	Width = GameBoard.GetWidth();
	Height = GameBoard.GetHeight();

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
			ATetrisBlock* NewBlock = GetWorld()->SpawnActor<ATetrisBlock>(BlockLocation, FRotator(0,0,0));
			NewBlock->SetCell(&GameBoard.At(BlockI, BlockJ));
			// Tell the block about its owner
			if (NewBlock != nullptr)
			{
				NewBlock->OwningGrid = this;
			}
		}
	}
}

void ATetrisBlockGrid::Tick(float DeltaSeconds)
{
	Tetris.GetCurrentMode().Tick(DeltaSeconds);

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

void ATetrisBlockGrid::AddScore()
{
	// Increment score
	Score++;

	UpdateText();
}

void ATetrisBlockGrid::HandleInput(tc::EGameplayInput i)
{
	Tetris.GetCurrentMode().RegisterInput(i);
}

void ATetrisBlockGrid::UpdateText()
{
	ScoreText->SetText(FText::Format(LOCTEXT("ScoreFmt", "Score: {0}"), FText::AsNumber(Score)));
}

#undef LOCTEXT_NAMESPACE

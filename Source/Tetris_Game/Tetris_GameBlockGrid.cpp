// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Tetris_GameBlockGrid.h"

#include "Tetris_GameBlock.h"


#include "Components/TextRenderComponent.h"
#include "Engine/World.h"
#include "ConstructorHelpers.h"

#define LOCTEXT_NAMESPACE "PuzzleBlockGrid"

#if WITH_EDITOR
#include "DrawDebugHelpers.h"
#endif


ATetris_GameBlockGrid::ATetris_GameBlockGrid()
	:Board()
{
#if WITH_EDITOR
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bTickEvenWhenPaused = true;
#endif

	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;

	// Create static mesh component
	ScoreText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("ScoreText0"));
	ScoreText->SetRelativeLocation(FVector(0.f,0.f,0.f));
	ScoreText->SetRelativeRotation(FRotator(90.f,0.f,0.f));
	ScoreText->SetupAttachment(DummyRoot);
	UpdateText();
}

void ATetris_GameBlockGrid::BeginPlay()
{
	Super::BeginPlay();

	Board.Reset(new tc::Board(Width, Height));

	Width = Board->GetWidth();
	Height = Board->GetHeight();

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
			ATetris_GameBlock* NewBlock = GetWorld()->SpawnActor<ATetris_GameBlock>(BlockLocation, FRotator(0,0,0));

			// Tell the block about its owner
			if (NewBlock != nullptr)
			{
				NewBlock->OwningGrid = this;
			}
		}
	}
}


void ATetris_GameBlockGrid::Tick(float DeltaSeconds)
{
	DrawDebugSphere(GetWorld(), FVector(20, 30, 40), 50, Width, FColor::Cyan);
}

void ATetris_GameBlockGrid::AddScore()
{
	// Increment score
	Score++;

	UpdateText();
}

void ATetris_GameBlockGrid::UpdateText()
{
	ScoreText->SetText(FText::Format(LOCTEXT("ScoreFmt", "Score: {0}"), FText::AsNumber(Score)));
}

#undef LOCTEXT_NAMESPACE

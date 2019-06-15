// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UniquePtr.h"
#include "core/details/Board.h"

#include "TetrisBlockGrid.generated.h"


/** Class used to spawn blocks and manage score */
UCLASS(minimalapi)
class ATetrisBlockGrid : public AActor
{
	GENERATED_BODY()

	/** Dummy root component */
	UPROPERTY(Category = Grid, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* DummyRoot;

	/** Text component for the score */
	UPROPERTY(Category = Grid, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UTextRenderComponent* ScoreText;

public:
	ATetrisBlockGrid();

	/** How many blocks have been clicked */
	int32 Score = 0;
	
	/** Width of the grid */
	UPROPERTY(Category=Grid, EditAnywhere, BlueprintReadOnly)
	int32 Width = 10;

	/** Height of the grid */
	UPROPERTY(Category=Grid, EditAnywhere, BlueprintReadOnly)
	int32 Height = 22;

	/** Spacing of blocks */
	UPROPERTY(Category=Grid, EditAnywhere, BlueprintReadOnly)
	float BlockSpacing = 300.0f;

protected:
	// Begin AActor interface
	virtual void BeginPlay() override;
#if WITH_EDITOR
	virtual void Tick(float DeltaSeconds) override;
	virtual bool ShouldTickIfViewportsOnly() const override { return true; }
#endif
	// End AActor interface

public:
	/** Handle the block being clicked */
	void AddScore();

private:
	void UpdateText();

private:
	TUniquePtr<tc::Board> Board;
};



// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "tc/TetrisHost.h"

#include "tc/modes/GameMode.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "TetrisBlockGrid.generated.h"

USTRUCT(BlueprintType)
struct FTetrisStats
{
	GENERATED_BODY()

	UPROPERTY(Category=Score, BlueprintReadOnly)
	int32 LineCount = 0;

	UPROPERTY(Category=Score, BlueprintReadOnly)
	int32 Level = 0;

	UPROPERTY(Category=Score, BlueprintReadOnly)
	int32 Score = 0;
};

/** Class used to spawn blocks and manage score */
UCLASS(MinimalAPI)
class ATetrisBlockGrid : public AActor
{
	GENERATED_BODY()
public:
	ATetrisBlockGrid();

	UPROPERTY(Category=Score, BlueprintReadOnly)
	FTetrisStats Stats;

	/** Width of the grid */
	UPROPERTY(Category=Grid, EditAnywhere, BlueprintReadOnly)
	int32 Width = 10;

	/** Height of the grid */
	UPROPERTY(Category=Grid, EditAnywhere, BlueprintReadOnly)
	int32 Height = 22;

	/** Spacing of blocks */
	UPROPERTY(Category=Grid, EditAnywhere, BlueprintReadOnly)
	float BlockSpacing = 300.0f;

	/** Scale of individual blocks */
	UPROPERTY(Category=Grid, EditAnywhere, BlueprintReadOnly)
	float BlockScaleFactor = 1.0f;

	/** Led light effect management */
	UPROPERTY(Category=Light, EditAnywhere)
	float IntensityPhantom = 1.0f;

	UPROPERTY(Category=Light, EditAnywhere)
	float IntensityLocked = 10.0f;

	UPROPERTY(Category=Light, EditAnywhere)
	float IntensityMove = 100.0f;

	float GetIntensityForState(tc::Cell Model)
	{
		return Model.state
			? Model.locked ? IntensityLocked : IntensityMove
			: Model.phantom ? IntensityPhantom : 0.0f;
	}

	UPROPERTY(Category=Color, EditAnywhere)
	FLinearColor ColorDefault = FColor::FromHex("FFE1AA");

	UPROPERTY(Category=Color, EditAnywhere)
	FLinearColor ColorI = FColor::FromHex("2bb8e2"); // 2bace2

	UPROPERTY(Category=Color, EditAnywhere)
	FLinearColor ColorL = FColor::FromHex("f89622");

	UPROPERTY(Category=Color, EditAnywhere)
	FLinearColor ColorJ = FColor::FromHex("1e4ca9"); // 005a9d

	UPROPERTY(Category=Color, EditAnywhere)
	FLinearColor ColorS = FColor::FromHex("4eb748");

	UPROPERTY(Category=Color, EditAnywhere)
	FLinearColor ColorZ = FColor::FromHex("ee2733");

	UPROPERTY(Category=Color, EditAnywhere)
	FLinearColor ColorO = FColor::FromHex("fde100");

	UPROPERTY(Category=Color, EditAnywhere)
	FLinearColor ColorT = FColor::FromHex("922b8c");

	FLinearColor GetColorForNature(tc::EPiece nature)
	{
		std::array<FLinearColor*, 7> Colors = { &ColorI, &ColorL, &ColorJ, &ColorS, &ColorZ, &ColorO, &ColorT };
		uint8 NatureIndex = uint8(nature);
		return NatureIndex < 7 ? *Colors[NatureIndex] : ColorDefault;
	}

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	// allow tick in real time rendering in editor
	virtual bool ShouldTickIfViewportsOnly() const override { return true; }

public:
	void HandleInput(tc::EGameplayInput i);

private:
	tc::TetrisHost Tetris;
	std::shared_ptr<tc::StandardGameMode> StdGameMode = nullptr;
};



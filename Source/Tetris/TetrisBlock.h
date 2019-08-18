// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TetrisBlock.generated.h"

namespace tc { struct Cell; }


/** A block that can be clicked */
UCLASS(MinimalAPI)
class ATetrisBlock : public AActor
{
	GENERATED_BODY()

	/** Dummy root component */
	UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* DummyRoot;

	/** StaticMesh component for the clickable block */
	UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* BlockMesh;

public:
	ATetrisBlock();

	/** Grid that owns us */
	UPROPERTY()
	class ATetrisBlockGrid* OwningGrid;

	/** Handle the block being clicked */
	UFUNCTION()
	void BlockClicked(UPrimitiveComponent* ClickedComp, FKey ButtonClicked);

	/** Handle the block being touched  */
	UFUNCTION()
	void OnFingerPressedBlock(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent);

	virtual void Tick(float DeltaSeconds) override;

	void SetCell(const tc::Cell* InReferredCell);

	void HandleClicked();

	void Highlight(bool bOn);

private:
	void UpdateMaterial();

private:
	UMaterialInstanceDynamic* LedMaterial;
	const tc::Cell* ReferredCell;
	int32 FaceIntensityParamIndex;
	int32 LightColorParamIndex;

	bool bIsHighlighted;
	float TargetIntensity;
	float CurrentIntensity;
	FLinearColor TargetLightColor;
};


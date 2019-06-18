// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "TetrisBlock.h"

#include "TetrisBlockGrid.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInstance.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/ConstructorHelpers.h"
#include "core/details/Board.h"

template<typename U>
U* FindAsset(const TCHAR* Path, bool bAssert=true)
{
	ConstructorHelpers::FObjectFinder<U> Ref(Path);
	ensureAlways(!bAssert || Ref.Object != nullptr);
	return Ref.Object;
}

ATetrisBlock::ATetrisBlock()
	: bIsActive(false)
	, LedMaterial(nullptr)
	, FaceIntensityParamIndex(INDEX_NONE)
	, ClickCount(0)
	, bIsHighlighted(false)
	, TargetIntensity(0)
	, CurrentIntensity(0)
	, ReferredCell(nullptr)
{
	// Structure to hold one-time initialization
	static auto CubeMeshRef     = FindAsset<UStaticMesh>      (TEXT("/Game/Puzzle/Meshes/PuzzleCube.PuzzleCube"));
	static auto BaseMaterialRef = FindAsset<UMaterial>        (TEXT("/Game/Puzzle/Meshes/BaseMaterial.BaseMaterial"));
	static auto LedMaterialRef  = FindAsset<UMaterialInstance>(TEXT("/Game/Materials/LedBlockOn.LedBlockOn"));

	// Save a pointer to the orange material
	HighlightMaterial = BaseMaterialRef;
	LedMaterial = UMaterialInstanceDynamic::Create(LedMaterialRef, nullptr);
	static const FName FaceIntensityParamName(TEXT("FaceIntensity"));
	float FaceIntensityDefault = 0.0f;
	ensure(LedMaterial->InitializeScalarParameterAndGetIndex(FaceIntensityParamName, FaceIntensityDefault, FaceIntensityParamIndex));

	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;

	// Create static mesh component
	BlockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlockMesh0"));
	BlockMesh->SetStaticMesh(CubeMeshRef);
	BlockMesh->SetRelativeScale3D(0.3f*FVector(1.f,1.f,0.25f));
	BlockMesh->SetRelativeLocation(FVector(0.f,0.f,25.f));
	BlockMesh->SetupAttachment(DummyRoot);
	BlockMesh->SetMaterial(0, LedMaterial);
	BlockMesh->OnClicked.AddDynamic(this, &ATetrisBlock::BlockClicked);
	BlockMesh->OnInputTouchBegin.AddDynamic(this, &ATetrisBlock::OnFingerPressedBlock);

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bTickEvenWhenPaused = true;
	PrimaryActorTick.TickGroup = TG_PrePhysics;
}

void ATetrisBlock::BlockClicked(UPrimitiveComponent* ClickedComp, FKey ButtonClicked)
{
	HandleClicked();
}


void ATetrisBlock::OnFingerPressedBlock(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent)
{
	HandleClicked();
}

void ATetrisBlock::HandleClicked()
{
	ClickCount++;
	ClickCount = ClickCount % 10;

	bIsActive = !bIsActive;

	TargetIntensity = bIsActive ? 100.0 : 0;

	// Tell the Grid
	if (OwningGrid != nullptr)
	{
		OwningGrid->AddScore();
	}
}

void ATetrisBlock::Highlight(bool bOn)
{
	bIsHighlighted = bOn;
	TargetIntensity = bOn ? 100 : 0;
}

void ATetrisBlock::UpdateMaterial()
{
	float Intensity = CurrentIntensity + 50 * bIsHighlighted;
	LedMaterial->SetScalarParameterByIndex(FaceIntensityParamIndex, Intensity);
}

void ATetrisBlock::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	const tc::Cell& Model = ReferredCell ? *ReferredCell : tc::Cell{};

// 	// update time dependent state
// 	float ChangeSpeed = 60 * DeltaSeconds * (bIsActive ? 1.0f : 0.2f);
//
// 	float Diff = TargetIntensity - CurrentIntensity;
//
// 	CurrentIntensity += Diff * FMath::Clamp<float>(ChangeSpeed, 0, 1);
	CurrentIntensity = Model.state * 100.0f;
	UpdateMaterial();
}

void ATetrisBlock::SetCell(const tc::Cell* InReferredCell)
{
	ReferredCell = InReferredCell;
}

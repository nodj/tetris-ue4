// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Tetris_GameBlock.h"
#include "Tetris_GameBlockGrid.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInstance.h"

template<typename U>
U* FindAsset(const TCHAR* Path, bool bAssert=true)
{
	ConstructorHelpers::FObjectFinder<U> Ref(Path);
	ensureAlways(!bAssert || Ref.Object != nullptr);
	return Ref.Object;
}

ATetris_GameBlock::ATetris_GameBlock()
	: TargetIntensity{0}
	, CurrentIntensity{0}
{
	// Structure to hold one-time initialization
	static auto PlaneMeshRef      = FindAsset<UStaticMesh>      (TEXT("/Game/Puzzle/Meshes/PuzzleCube.PuzzleCube"));
	static auto BaseMaterialRef   = FindAsset<UMaterial>        (TEXT("/Game/Puzzle/Meshes/BaseMaterial.BaseMaterial"));
	static auto LedMaterialRef    = FindAsset<UMaterialInstance>(TEXT("/Game/Materials/LedBlockOn.LedBlockOn"));

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
	BlockMesh->SetStaticMesh(PlaneMeshRef);
	BlockMesh->SetRelativeScale3D(0.3f*FVector(1.f,1.f,0.25f));
	BlockMesh->SetRelativeLocation(FVector(0.f,0.f,25.f));
	BlockMesh->SetupAttachment(DummyRoot);
	BlockMesh->SetMaterial(0, LedMaterial);
	BlockMesh->OnClicked.AddDynamic(this, &ATetris_GameBlock::BlockClicked);
	BlockMesh->OnInputTouchBegin.AddDynamic(this, &ATetris_GameBlock::OnFingerPressedBlock);

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bTickEvenWhenPaused = true;
	PrimaryActorTick.TickGroup = TG_PrePhysics;
}

void ATetris_GameBlock::BlockClicked(UPrimitiveComponent* ClickedComp, FKey ButtonClicked)
{
	HandleClicked();
}


void ATetris_GameBlock::OnFingerPressedBlock(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent)
{
	HandleClicked();
}

void ATetris_GameBlock::HandleClicked()
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

void ATetris_GameBlock::Highlight(bool bOn)
{
	bIsHighlighted = bOn;
	TargetIntensity = bOn ? 100 : 0;
}

void ATetris_GameBlock::UpdateMaterial()
{
	float Intensity = CurrentIntensity + 50 * bIsHighlighted;
	LedMaterial->SetScalarParameterByIndex(FaceIntensityParamIndex, Intensity);
}

void ATetris_GameBlock::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// update time dependant state
	float ChangeSpeed = 60 * DeltaSeconds * (bIsActive ? 1.0f : 0.2f);

	float Diff = TargetIntensity - CurrentIntensity;

	CurrentIntensity += Diff * FMath::Clamp<float>(ChangeSpeed, 0, 1);

	UpdateMaterial();
}

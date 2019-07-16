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
	: LedMaterial(nullptr)
	, ReferredCell(nullptr)
	, FaceIntensityParamIndex(INDEX_NONE)
	, bIsHighlighted(false)
	, TargetIntensity(0)
	, CurrentIntensity(0)
	, TargetLightColor(0.0f, 0.0f, 0.0f)
{
	// Structure to hold one-time initialization
	static auto CubeMeshRef = FindAsset<UStaticMesh>(TEXT("/Game/Puzzle/Meshes/PuzzleCube.PuzzleCube"));
	static auto BaseMaterialRef = FindAsset<UMaterial>(TEXT("/Game/Puzzle/Meshes/BaseMaterial.BaseMaterial"));
	static auto LedMaterialRef = FindAsset<UMaterialInstance>(TEXT("/Game/Materials/LedBlockOn.LedBlockOn"));

	// Save a pointer to the orange material
	HighlightMaterial = BaseMaterialRef;
	LedMaterial = UMaterialInstanceDynamic::Create(LedMaterialRef, nullptr);
	static const FName FaceIntensityParamName("FaceIntensity");
	static const FName LightColorParamName("LightColor");
	float FaceIntensityDefault = 0.0f;
	FLinearColor LightColorDefault(1.0f, 1.0f, 1.0f);
	ensure(LedMaterial->InitializeScalarParameterAndGetIndex(FaceIntensityParamName, FaceIntensityDefault, FaceIntensityParamIndex));
	ensure(LedMaterial->InitializeVectorParameterAndGetIndex(LightColorParamName, LightColorDefault, FaceIntensityParamIndex));

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

	LedMaterial->SetVectorParameterByIndex(LightColorParamIndex, TargetLightColor);
}

constexpr FLinearColor GetColorForNature(tc::EPiece nature)
{
	switch(nature)
	{
		case tc::EPiece::Piece_I: return FColor::Cyan;
		case tc::EPiece::Piece_L: return FColor::Orange;
		case tc::EPiece::Piece_J: return FColor::Blue;
		case tc::EPiece::Piece_S: return FColor::Green;
		case tc::EPiece::Piece_Z: return FColor::Red;
		case tc::EPiece::Piece_O: return FColor::Yellow;
		case tc::EPiece::Piece_T: return FColor::Purple;
		default: return FColor::White;
	}
}

void ATetrisBlock::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	const tc::Cell& Model = ReferredCell ? *ReferredCell : tc::Cell{};
	TargetIntensity = Model.state ? Model.locked? 10.0f : 100.0f : 0.0f;
	if (Model.state || Model.phantom)
		TargetLightColor = GetColorForNature(Model.nature);

	// update time dependent state
	float ChangeSpeed = 60 * DeltaSeconds * (Model.state ? 1.0f : 0.35f);

	float Diff = TargetIntensity - CurrentIntensity;

	CurrentIntensity += Diff * FMath::Clamp<float>(ChangeSpeed, 0, 1);
	UpdateMaterial();
}

void ATetrisBlock::SetCell(const tc::Cell* InReferredCell)
{
	ReferredCell = InReferredCell;
}

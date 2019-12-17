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
	static auto CubeMeshRef = FindAsset<UStaticMesh>(TEXT("/Game/Geometry/Meshes/1M_Cube_Chamfer.1M_Cube_Chamfer"));
	static auto LedMaterialRef = FindAsset<UMaterialInstance>(TEXT("/Game/Materials/LedBlockOn.LedBlockOn"));

	// Save a pointer to the orange material
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
	BlockMesh->SetRelativeScale3D(FVector(1.0f, 1.0f, 0.25f));
	BlockMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 25.0f));
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

void ATetrisBlock::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	const tc::Cell& Model = ReferredCell ? *ReferredCell : tc::Cell{};
	TargetIntensity = OwningGrid ? OwningGrid->GetIntensityForState(Model) : 1.0f;
	if (Model.state || Model.phantom)
		TargetLightColor = OwningGrid ? OwningGrid->GetColorForNature(Model.nature) : FColor::White;

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

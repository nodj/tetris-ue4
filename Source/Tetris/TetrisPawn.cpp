// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "TetrisPawn.h"

#include "TetrisBlock.h"
#include "TetrisGameInstance.h"
#include "TetrisBlockGrid.h"

#include "tc/modes/GameMode.h"

#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"


ATetrisPawn::ATetrisPawn(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

void ATetrisPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		FVector Start, Dir;
		PC->DeprojectMousePositionToWorld(Start, Dir);
		FVector End = Start + (Dir * 8000.0f);
		TraceForBlock(Start, End, false);
	}
}

void ATetrisPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("RotateLeft", EInputEvent::IE_Pressed, this, &ATetrisPawn::OnRotateLeft);
	PlayerInputComponent->BindAction("RotateRight", EInputEvent::IE_Pressed, this, &ATetrisPawn::OnRotateRight);
	PlayerInputComponent->BindAction("MoveLeft", EInputEvent::IE_Pressed, this, &ATetrisPawn::OnMoveLeftPressed);
	PlayerInputComponent->BindAction("MoveLeft", EInputEvent::IE_Released, this, &ATetrisPawn::OnMoveLeftReleased);
	PlayerInputComponent->BindAction("MoveRight", EInputEvent::IE_Pressed, this, &ATetrisPawn::OnMoveRightPressed);
	PlayerInputComponent->BindAction("MoveRight", EInputEvent::IE_Released, this, &ATetrisPawn::OnMoveRightReleased);
	PlayerInputComponent->BindAction("MoveDown", EInputEvent::IE_Pressed, this, &ATetrisPawn::OnMoveDownPressed);
	PlayerInputComponent->BindAction("MoveDown", EInputEvent::IE_Released, this, &ATetrisPawn::OnMoveDownReleased);

	PlayerInputComponent->BindAction("Start", EInputEvent::IE_Pressed, this, &ATetrisPawn::OnStartPressed);
	PlayerInputComponent->BindAction("Select", EInputEvent::IE_Pressed, this, &ATetrisPawn::OnSelectPressed);
	PlayerInputComponent->BindAction("HardDrop", EInputEvent::IE_Pressed, this, &ATetrisPawn::OnHardDropPressed);
	PlayerInputComponent->BindAction("Hold", EInputEvent::IE_Pressed, this, &ATetrisPawn::OnHoldPressed);

	PlayerInputComponent->BindAction("dbgDump", EInputEvent::IE_Pressed, this, &ATetrisPawn::OnHoldPressed);

// 	PlayerInputComponent->BindAxis("MoveHzAxis", this, &ATetrisPawn::OnMoveHzAxis);
}

void ATetrisPawn::CalcCamera(float DeltaTime, struct FMinimalViewInfo& OutResult)
{
	Super::CalcCamera(DeltaTime, OutResult);

	OutResult.Rotation = FRotator(-90.0f, -90.0f, 0.0f);
}

void ATetrisPawn::DelegateEvent(tc::EGameplayInput i)
{
	if (UTetrisGameInstance* GI = GetGameInstance<UTetrisGameInstance>())
	{
		if (ATetrisBlockGrid* Grid = GI->TheGrid.Get())
		{
			Grid->HandleInput(i);
		}
	}
}

void ATetrisPawn::TraceForBlock(const FVector& Start, const FVector& End, bool bDrawDebugHelpers)
{
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility);
	if (bDrawDebugHelpers)
	{
		DrawDebugLine(GetWorld(), Start, HitResult.Location, FColor::Red);
		DrawDebugSolidBox(GetWorld(), HitResult.Location, FVector(20.0f), FColor::Red);
	}
	if (HitResult.Actor.IsValid())
	{
		ATetrisBlock* HitBlock = Cast<ATetrisBlock>(HitResult.Actor.Get());
		if (CurrentBlockFocus != HitBlock)
		{
			if (CurrentBlockFocus)
			{
				CurrentBlockFocus->Highlight(false);
			}
			if (HitBlock)
			{
				HitBlock->Highlight(true);
			}
			CurrentBlockFocus = HitBlock;
		}
	}
	else if (CurrentBlockFocus)
	{
		CurrentBlockFocus->Highlight(false);
		CurrentBlockFocus = nullptr;
	}
}
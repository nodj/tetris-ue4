// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "core/modes/GameMode.h"
#include "core/details/GameplayInput.h"
#include "TetrisPawn.generated.h"

UCLASS(config=Game)
class ATetrisPawn : public APawn
{
	GENERATED_UCLASS_BODY()

public:
	virtual void Tick(float DeltaSeconds) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual void CalcCamera(float DeltaTime, struct FMinimalViewInfo& OutResult) override;

protected:
	void TraceForBlock(const FVector& Start, const FVector& End, bool bDrawDebugHelpers);

	void DelegateEvent(tc::EGameplayInput i);
	void OnStartPressed() { DelegateEvent(tc::EGameplayInput::Start); }
	void OnSelectPressed() { DelegateEvent(tc::EGameplayInput::Select); }
	void OnRotateLeft() { DelegateEvent(tc::EGameplayInput::RotateLeft); }
	void OnRotateRight() { DelegateEvent(tc::EGameplayInput::RotateRight); }
	void OnHardDropPressed() { DelegateEvent(tc::EGameplayInput::HardDrop); }
	void OnMoveLeftPressed() { DelegateEvent(tc::EGameplayInput::MoveLeftPressed); }
	void OnMoveLeftReleased() { DelegateEvent(tc::EGameplayInput::MoveLeftReleased); }
	void OnMoveRightPressed() { DelegateEvent(tc::EGameplayInput::MoveRightPressed); }
	void OnMoveRightReleased() { DelegateEvent(tc::EGameplayInput::MoveRightReleased); }

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	class ATetrisBlock* CurrentBlockFocus;
};

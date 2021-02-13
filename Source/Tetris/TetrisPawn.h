// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "tc/modes/GameMode.h"
#include "tc/details/GameplayInput.h"
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
	void OnHoldPressed() { DelegateEvent(tc::EGameplayInput::Hold); }
	void OnMoveLeftPressed() { DelegateEvent(tc::EGameplayInput::MoveLeftPressed); }
	void OnMoveLeftReleased() { DelegateEvent(tc::EGameplayInput::MoveLeftReleased); }
	void OnMoveRightPressed() { DelegateEvent(tc::EGameplayInput::MoveRightPressed); }
	void OnMoveRightReleased() { DelegateEvent(tc::EGameplayInput::MoveRightReleased); }
	void OnMoveDownPressed() { DelegateEvent(tc::EGameplayInput::MoveDownPressed); }
	void OnMoveDownReleased() { DelegateEvent(tc::EGameplayInput::MoveDownReleased); }

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	class ATetrisBlock* CurrentBlockFocus;
};

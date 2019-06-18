#pragma once

#include "Engine/GameInstance.h"

#include "TetrisGameInstance.generated.h"

/**
 * Acts like a global stucture, live from the begining of a game, until the complete end.
 * I store usefull references here for fast access, instead of searches operations
 */
UCLASS()
class UTetrisGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UTetrisGameInstance();
	~UTetrisGameInstance();

	TWeakObjectPtr<class ATetrisBlockGrid> TheGrid = nullptr;
};

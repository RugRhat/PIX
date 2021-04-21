// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MenuInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMenuInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PIX_API IMenuInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// ALL EMPTY IMPLEMENTATIONS.
	virtual void Host() = 0;			

	virtual void Join(const FString& Address) = 0;

	virtual void SetGameMode(const FString& GameMode) = 0;

	virtual void SetGameModeID(const FString& GameModeID) = 0;

	virtual void LoadMainMenu() = 0;

	virtual void LoadGame() = 0;

	virtual void BackToLobby() = 0;

	virtual void Replay() = 0;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "PIXGameState.generated.h"

/**
 * 
 */
UCLASS()
class PIX_API APIXGameState : public AGameState
{
	GENERATED_BODY()

protected:
	// Prevents player movement during pre-match.
	virtual void HandleMatchIsWaitingToStart() override;

	// Enables player movement.
	virtual void HandleMatchHasStarted() override;

	virtual void HandleMatchHasEnded() override;

	// Called when game/match state is replicated.
	virtual void OnRep_MatchState() override;

	// Event functionality assigned in blueprints.
	UFUNCTION(BlueprintImplementableEvent, Category = "GameState")
	void GameStateChanged(FName NewState);	
};

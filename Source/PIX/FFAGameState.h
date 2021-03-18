// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "FFAGameState.generated.h"

// Allows for blueprint UI functionality.
UENUM(BlueprintType)
enum class EFFAState : uint8
{
	WaitingToBegin,

	MatchStarted,

	GameOver,
};

UCLASS()
class PIX_API AFFAGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	void SetGameState(EFFAState NewState);

protected:
	UFUNCTION()
	void OnRep_GameState(EFFAState OldState);

	UFUNCTION(BlueprintImplementableEvent, Category = "GameState")
	void GameStateChanged(EFFAState NewState, EFFAState OldState);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_GameState, Category = "GameState")
	EFFAState GameState;
};

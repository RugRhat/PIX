// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "HordeGameState.generated.h"


// Allows for blueprint UI functionality.
UENUM(BlueprintType)
enum class EHordeState : uint8
{
	WaitingToBegin,

	WaveStarted,

	WaveComplete,

	GameOver,

};


UCLASS()
class PIX_API AHordeGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	void SetGameState(EHordeState NewState);

	UFUNCTION(BlueprintCallable, Category = "GameState")
	void IncreaseTotalKillCount();

	UPROPERTY(BlueprintReadOnly)
	int32 TotalKillCount = 0;
	
protected:
	UFUNCTION()
	void OnRep_GameState(EHordeState OldState);

	UFUNCTION(BlueprintImplementableEvent, Category = "GameState")
	void GameStateChanged(EHordeState NewState, EHordeState OldState);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_GameState, Category = "GameState")
	EHordeState GameState;

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FFAGameMode.generated.h"

enum class EFFAState : uint8;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnActorKilledFFA, APawn*, Victim, AController*, KillerController);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameWon, AController*, WinnerController);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameLost, AController*, LoserController);

UCLASS()
class PIX_API AFFAGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public: 
	// Sets default values for this gamemode's properties.
	AFFAGameMode();

	// Dynamic event that is assigned through blueprint.
	UPROPERTY(BlueprintAssignable, Category = "GameMode")
	FOnActorKilledFFA OnActorKilled;

	// Dynamic event that is assigned through blueprint.
	UPROPERTY(BlueprintAssignable, Category = "GameMode")
	FOnGameWon Winner;

	// Dynamic event that is assigned through blueprint.
	UPROPERTY(BlueprintAssignable, Category = "GameMode")
	FOnGameLost Loser;

protected:
	// Max number of kills required to win game.
	UPROPERTY(EditDefaultsOnly)
	uint32 MaxKills = 15;

	// Determines delay time before starting game. Can be changed from editor.
	UPROPERTY(EditDefaultsOnly, Category = "GameMode")
	float StartMatchDelay;

	// Determines time between waves. Can be changed from editor.
	UPROPERTY(EditDefaultsOnly, Category = "GameMode")
	float RespawnDelay;

	// Checks if max kill count reached.
	void CheckKillCount();

	// Handles starting game match.
	void StartMatch();

	UFUNCTION(BlueprintCallable, Category = "GameMode")
	void SetPlayerTeams();

	// Handles game over.
	void HandleGameOver();

	// Sets new game state.
	void SetGameState(EFFAState NewState);

	// Called when game starts, if gamemode selected.
	virtual void BeginPlay() override;

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	FTimerHandle TimerHandle_StartMatch;					// Timer for starting a new match.
	FTimerHandle TimerHandle_RespawnDeadPlayer;				// Timer for respawning dead player.
};

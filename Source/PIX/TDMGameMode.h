// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "TDMGameMode.generated.h"

// Called by health component when a player dies.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnActorKilledTDM, APawn*, Victim, AController*, KillerController);

UCLASS()
class PIX_API ATDMGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	// Sets default values for this gamemode's properties.
	ATDMGameMode();

	// Event called by health component when a player dies that is assigned through blueprint.
	UPROPERTY(BlueprintAssignable, Category = "GameMode")
	FOnActorKilledTDM OnActorKilled;

	// Checks if max kill count reached.
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void CheckKillCount();

protected:
	// Randomly places each player on a team.
	UFUNCTION(BlueprintCallable, Category = "Gameplay") 
	void SetPlayerTeams();

	// Max number of kills required to win game.
	UPROPERTY(EditDefaultsOnly)
	uint32 MaxKills = 15;

	// Team 1's kills.
	UPROPERTY(BlueprintReadOnly)
	int32 Kills_T1 = 0;

	// Team 1's kills.
	UPROPERTY(BlueprintReadOnly)
	int32 Kills_T2 = 0;

	// Determines delay time before starting game. Can be changed from editor.
	UPROPERTY(EditDefaultsOnly, Category = "GameMode")
	float StartMatchDelay;

	// Determines delay time before setting player teams. Can be changed from editor.
	UPROPERTY(EditDefaultsOnly, Category = "GameMode")
	float SetTeamsDelay;

	UFUNCTION(BlueprintCallable, Category = "GameMode")
	void IncreaseTeamKillCount(int TeamNum);

	// Changes match state to cue intro and starts countdown to start of match.
	void CueGameIntro();

	// Handles game over.
	void HandleGameOver(int WinningTeamNum);

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	// Handles starting game match.
	virtual void StartMatch() override;

	// Called when the game starts or when spawned.
	virtual void BeginPlay() override;

	int TeamCount_1;
	int TeamCount_2;

	FTimerHandle TimerHandle_StartMatch;					// Timer for starting a new match.
	FTimerHandle TimerHandle_SetTeams;						// Timer for setting player teams.
};

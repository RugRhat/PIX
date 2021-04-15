// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "HordeGameMode.generated.h"

// Called by health component when a character dies.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnActorKilledH, APawn*, Victim, AController*, KillerController);

UCLASS()
class PIX_API AHordeGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	// Sets default values for this gamemode's properties.
	AHordeGameMode();

	// Event called by health component when a character dies that is assigned through blueprint.
	UPROPERTY(BlueprintAssignable, Category = "Gameplay")
	FOnActorKilledH OnActorKilled;

	// Handles player health regeneration if any players alive. If no players alive, ends game. 
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void PlayersAlive();

protected:
	// Creates a class of Enemy Characters. Horde BP set in editor.
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ABaseCharacter> EnemyCharacter;

	// Creates a class of spawn points for Horde AI. SpawnPoint BP set in editor.
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> SpawnPoints;

	// Determines the base number of enemies to add each wave. 
	// Value multiplied by wave number. Can be changed from editor.
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	int32 BaseNoOfEnemies = 5;

	// Determines delay time before starting game. Can be changed from editor.
	UPROPERTY(EditDefaultsOnly, Category = "GameMode")
	float StartMatchDelay;

	// Determines time between waves. Can be changed from editor.
	UPROPERTY(EditDefaultsOnly, Category = "GameMode")
	float TimeBetweenWaves;

	// Determines time between waves. Can be changed from editor.
	UPROPERTY(EditDefaultsOnly, Category = "GameMode")
	float RespawnDelay;

	UPROPERTY(BlueprintReadOnly, Category = "Gameplay")
	int32 TeamKills = 0;
	
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void IncreaseTeamKillCount();

	// Handles spawning of enemies.
	void SpawnEnemy();

	void StartWave();
	void NewWave();			// Increases no. of enemies & wave no.
	void EndWave();

	// Changes match state to cue intro and starts countdown to start of match.
	void CueGameIntro();

	// Checks if any enemies alive. If not, a new wave begins.
	void EnemiesAlive();

	// Handles player respawning.
	void RespawnPlayers();

	// Determines when to spawn enemies.
	void EnemySpawner();

	// Handles game loss.
	void HandleGameOver();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	// Handles starting game match.
	virtual void StartMatch() override;

	// Called when the game starts or when spawned.
	virtual void BeginPlay() override;

	TArray<AActor*> SpawnLocations;						// Array of all Horde spawn points in current map.

	int32 Wave;											// Current wave number.
	int32 EnemiesToSpawn;								// Current number of enemies to spawn.

	FTimerHandle TimerHandle_StartMatch;				// Timer for starting match.
	FTimerHandle TimerHandle_StartNewWave;				// Timer for starting a new wave.
	FTimerHandle TimerHandle_EnemySpawner;				// Timer for spawning enemies.
	FTimerHandle TimerHandle_RespawnDeadPlayers;		// Timer for respawning dead players.
};

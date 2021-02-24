// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HordeGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PIX_API AHordeGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	// Sets default values for this gamemode's properties.
	AHordeGameMode();

	// UFUNCTION(BlueprintImplementableEvent)
	// void GameOver(bool PlayerWon);

	// UFUNCTION(BlueprintCallable)
	// int GetWave();

protected:
	// Creates a class of Enemy Characters. Horde BP set in editor.
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ABaseCharacter> EnemyCharacter;

	// Creates a class of spawn points for Horde AI. SpawnPoint BP set in editor.
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> SpawnPoints;

	// Determines the base number of enemies to add each wave. 
	// Value multiplied by wave number. Can be changed from editor.
	UPROPERTY(EditDefaultsOnly, Category = "Enemies")
	int32 BaseNoOfEnemies = 5;

	// Determines time between waves. Can be changed from editor.
	UPROPERTY(EditDefaultsOnly, Category = "GameMode")
	float TimeBetweenWaves;
	
	// Handles spawning of enemies.
	void SpawnEnemy();

	void StartWave();
	void NewWave();
	void EndWave();

	// Handles player health regeneration if any players alive. If no players alive, ends game. 
	void PlayersAlive();

	// Checks if any enemies alive. If not, a new wave begins.
	void EnemiesAlive();

	// Handles player respawning.
	void RespawnPlayers();

	// Determines when to spawn enemies.
	void EnemySpawner();

	// Handles game loss.
	void HandleGameOver();

	// Called when game starts, if gamemode selected.
	virtual void BeginPlay() override;

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	TArray<AActor*> SpawnLocations;						// Array of all Horde spawn points in current map.

	int32 Wave;											// Current wave number.
	int32 EnemiesToSpawn;								// Current number of enemies to spawn.

	FTimerHandle TimerHandle_EnemySpawner;				// Timer for spawning enemies.
	FTimerHandle TimerHandle_StartNewWave;				// Timer for starting a new wave.
};

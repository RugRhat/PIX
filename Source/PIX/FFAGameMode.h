// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "FFAGameMode.generated.h"

// Called by health component when a player dies.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnActorKilledFFA, APawn*, Victim, AController*, KillerController);

UCLASS()
class PIX_API AFFAGameMode : public AGameMode
{
	GENERATED_BODY()

public: 
	// Sets default values for this gamemode's properties.
	AFFAGameMode();

	// Event called by health component when a player dies that is assigned through blueprint.
	UPROPERTY(BlueprintAssignable, Category = "Gameplay")
	FOnActorKilledFFA OnActorKilled;

	// Checks if max kill count reached.
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void CheckKillCount();

protected:
	// Max number of kills required to win game.
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	uint32 MaxKills = 15;

	// Determines delay time before starting game. Can be changed from editor.
	UPROPERTY(EditDefaultsOnly, Category = "GameMode")
	float StartMatchDelay;

	// Determines delay time before setting player teams. Can be changed from editor.
	UPROPERTY(EditDefaultsOnly, Category = "GameMode")
	float SetTeamsDelay;

	// Places each player on a unique team.
	UFUNCTION(BlueprintCallable, Category = "Gameplay") 
	void SetPlayerTeams();

	// Changes match state to cue intro and starts countdown to start of match.
	void CueGameIntro();

	// Handles game over.
	void HandleGameOver(class APlayerController* PlayerController);

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	// Handles starting game match.
	virtual void StartMatch() override;

	// Called when the game starts or when spawned.
	virtual void BeginPlay() override;

	FTimerHandle TimerHandle_StartMatch;					// Timer for starting a new match.
	FTimerHandle TimerHandle_SetTeams;						// Timer for setting player teams.
};

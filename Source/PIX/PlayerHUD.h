// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

// Called by health component when a player dies.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPlayerDied, APawn*, Victim, AController*, VictimController);

// Called at the end of a match if a player wins.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameWon, AController*, WinnerController);

// Called at the end of a match if a player loses.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameLost, AController*, LoserController);

// Called by Horde Game Mode when a player respawns.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerRespawned, AController*, RespawnedPlayerController);

UCLASS()
class PIX_API APlayerHUD : public AHUD
{
	GENERATED_BODY()

public:
	// Event called by health component when a player dies that is assigned through blueprint.
	UPROPERTY(BlueprintAssignable, Category = "HUD")
	FOnPlayerDied OnPlayerDied;

	// Event called by health component when a player dies that is assigned through blueprint.
	UPROPERTY(BlueprintAssignable, Category = "HUD")
	FOnPlayerRespawned OnRespawned;

	// Event called at then end of a match if a player wins that is assigned through blueprint.
	UPROPERTY(BlueprintAssignable, Category = "HUD")
	FOnGameWon Winner;

	// Event called at then end of a match if a player loses that is assigned through blueprint.
	UPROPERTY(BlueprintAssignable, Category = "HUD")
	FOnGameLost Loser;
};

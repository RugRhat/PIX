// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "DefaultPlayerState.generated.h"

UCLASS()
class PIX_API ADefaultPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	// Called by player/actor death event to increase player kill count.
	UFUNCTION(BlueprintCallable, Category = "Kill/Death")
	void IncreaseKillCount();

	// Called by player/actor death event to increase player death count.
	UFUNCTION(BlueprintCallable, Category = "Kill/Death")
	void IncreaseDeathCount();

	UPROPERTY(BlueprintReadOnly, Category = "Kill/Death")
	int32 KillCount = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Kill/Death")
	int32 DeathCount = 0;
};

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
	UFUNCTION(BlueprintCallable, Category = "PlayerState")
	void IncreaseKillCount();

	UFUNCTION(BlueprintCallable, Category = "PlayerState")
	void IncreaseDeathCount();

	UPROPERTY(BlueprintReadOnly)
	int32 KillCount = 0;

	UPROPERTY(BlueprintReadOnly)
	int32 DeathCount = 0;
};

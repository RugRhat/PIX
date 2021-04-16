// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "HordeAI.generated.h"

UCLASS()
class PIX_API AHordeAI : public AAIController
{
	GENERATED_BODY()
	
public:
	virtual void Tick(float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere)
	class UBehaviorTree* AIBehavior;

	// Finds all the player characters using the BP_Player class.
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> PotentialPlayers;

	// List of all players in current game.
	TArray<AActor*> Players;

	float AttackRange = 100000.0f;

	virtual void BeginPlay() override;
};

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
	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere)
	class UBehaviorTree* AIBehavior;

	// Max AI attack distance.
	float AttackRange = 100000.0f;

	// Called when the game starts or when spawned.
	virtual void BeginPlay() override;
};

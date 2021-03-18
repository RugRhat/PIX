// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPlayerDied, APawn*, Victim, AController*, VictimController);

UCLASS()
class PIX_API APlayerHUD : public AHUD
{
	GENERATED_BODY()

public:
	// Dynamic event that is assigned through blueprint.
	UPROPERTY(BlueprintAssignable, Category = "HUD")
	FOnPlayerDied OnPlayerDied;
};

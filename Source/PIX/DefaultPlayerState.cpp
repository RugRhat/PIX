// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultPlayerState.h"


// Called by player/actor death event to increase player kill count.
void ADefaultPlayerState::IncreaseKillCount() 
{
    KillCount ++;

    // For debugging.
    UE_LOG(LogTemp, Warning, TEXT("Player Kill Count: %d"), KillCount);
}

// Called by player/actor death event to increase player death count.
void ADefaultPlayerState::IncreaseDeathCount() 
{
    DeathCount ++;

    // For debugging.
    UE_LOG(LogTemp, Warning, TEXT("Player Death Count: %d"), DeathCount);
}

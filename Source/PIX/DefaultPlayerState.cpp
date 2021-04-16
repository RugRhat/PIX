// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultPlayerState.h"


void ADefaultPlayerState::IncreaseKillCount() 
{
    KillCount ++;

    // For debugging.
    UE_LOG(LogTemp, Warning, TEXT("Player Kill Count: %d"), KillCount);
}

void ADefaultPlayerState::IncreaseDeathCount() 
{
    DeathCount ++;

    // For debugging.
    UE_LOG(LogTemp, Warning, TEXT("Player Death Count: %d"), DeathCount);
}



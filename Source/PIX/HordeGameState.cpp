// Fill out your copyright notice in the Description page of Project Settings.


#include "HordeGameState.h"
#include "Net/UnrealNetwork.h"

void AHordeGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AHordeGameState, GameState);
}

void AHordeGameState::SetGameState(EHordeState NewState) 
{
    // Only server can change state
    if (HasAuthority())
	{
		EHordeState OldState = GameState;

		GameState = NewState;

		// Call on server
		OnRep_GameState(OldState);
	}
}

void AHordeGameState::OnRep_GameState(EHordeState OldState) 
{
    GameStateChanged(GameState, OldState);
}

void AHordeGameState::IncreaseTotalKillCount() 
{
    TotalKillCount ++;
    
    // For debugging.
    UE_LOG(LogTemp, Warning, TEXT("Total Kill Count: %d"), TotalKillCount);
}

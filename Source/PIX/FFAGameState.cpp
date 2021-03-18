// Fill out your copyright notice in the Description page of Project Settings.


#include "FFAGameState.h"
#include "Net/UnrealNetwork.h"


void AFFAGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFFAGameState, GameState);
}

void AFFAGameState::SetGameState(EFFAState NewState) 
{
    // Only server can change state
    if (HasAuthority())
	{
		EFFAState OldState = GameState;

		GameState = NewState;

		// Call on server
		OnRep_GameState(OldState);
	}
}

void AFFAGameState::OnRep_GameState(EFFAState OldState) 
{
    GameStateChanged(GameState, OldState);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "PIXGameState.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerCharacter.h"


// Prevents player movement during pre-match.
void APIXGameState::HandleMatchIsWaitingToStart() 
{
    Super::HandleMatchIsWaitingToStart();

    // for (FConstPlayerControllerIterator ControlledPlayer = GetWorld()->GetPlayerControllerIterator(); ControlledPlayer; ++ControlledPlayer)
	// {
	// 	APlayerController* PlayerController = ControlledPlayer->Get();
	// 	if (PlayerController->GetPawn())
	// 	{
    //         APlayerCharacter* PlayerChar = Cast<APlayerCharacter>(PlayerController->GetPawn());
    //         if(ensure(PlayerChar)) return;

    //         PlayerChar->MovementComponent->DisableMovement();
    //     }
    // }
}

// Enables player movement.
void APIXGameState::HandleMatchHasStarted() 
{
    Super::HandleMatchHasStarted();

    // for (FConstPlayerControllerIterator ControlledPlayer = GetWorld()->GetPlayerControllerIterator(); ControlledPlayer; ++ControlledPlayer)
	// {
	// 	APlayerController* PlayerController = ControlledPlayer->Get();
	// 	if (PlayerController->GetPawn())
	// 	{
    //         APlayerCharacter* PlayerChar = Cast<APlayerCharacter>(PlayerController->GetPawn());
    //         if(ensure(PlayerChar)) return;

    //         PlayerChar->MovementComponent->SetMovementMode(MOVE_Walking);
    //     }
    // }
}

void APIXGameState::HandleMatchHasEnded() 
{
    Super::HandleMatchHasEnded();
    /// TODO: Set input to mouse only & destroy pawns.
}

// Called when game/match state is replicated.
void APIXGameState::OnRep_MatchState() 
{
    Super::OnRep_MatchState();

    GameStateChanged(GetMatchState()); 
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "FFAGameMode.h"
#include "DefaultPlayerState.h"
#include "EngineUtils.h"
#include "FFAGameState.h"
#include "HealthComponent.h"
#include "PlayerCharacter.h"
#include "TimerManager.h"


AFFAGameMode::AFFAGameMode() 
{
    RespawnDelay = 5.0f;
    StartMatchDelay = 8.0f;

    // Sets Default Player State as game mode's player state.
    PlayerStateClass = ADefaultPlayerState::StaticClass();

    PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 1.0f;
}

void AFFAGameMode::BeginPlay() 
{
    Super::BeginPlay();

    SetGameState(EFFAState::WaitingToBegin);

    GetWorldTimerManager().SetTimer(TimerHandle_StartMatch, this, &AFFAGameMode::StartMatch, StartMatchDelay, false);
}

void AFFAGameMode::SetGameState(EFFAState NewState) 
{
    AFFAGameState* ThisGameState = GetGameState<AFFAGameState>();
	if (ensureAlways(ThisGameState))
	{
		ThisGameState->SetGameState(NewState);
	}
}

void AFFAGameMode::Tick(float DeltaSeconds) 
{
    Super::Tick(DeltaSeconds);

    CheckKillCount();
}

void AFFAGameMode::StartMatch() 
{
    SetPlayerTeams();

    SetGameState(EFFAState::MatchStarted);

    SetActorTickEnabled(true);    
}

///TODO: Figure out better way to do this.
void AFFAGameMode::SetPlayerTeams() 
{
    int Team = 1;
    for (FConstPlayerControllerIterator ControlledPlayer = GetWorld()->GetPlayerControllerIterator(); ControlledPlayer; ++ControlledPlayer)
	{
		APlayerController* PlayerController = ControlledPlayer->Get();
		if (PlayerController->GetPawn())
		{
            APawn* MyPawn = PlayerController->GetPawn();
			UHealthComponent* HealthComp = Cast<UHealthComponent>(MyPawn->GetComponentByClass(UHealthComponent::StaticClass()));

            if(ensure(HealthComp))
            {
                HealthComp->TeamNum = Team;
                Team ++;

                UE_LOG(LogTemp, Warning, TEXT("Team No. : %d"), HealthComp->TeamNum);
            }
        }
    }
}

void AFFAGameMode::CheckKillCount() 
{
    for (FConstPlayerControllerIterator ControlledPlayer = GetWorld()->GetPlayerControllerIterator(); ControlledPlayer; ++ControlledPlayer)
	{
		APlayerController* PlayerController = ControlledPlayer->Get();
		if (PlayerController->GetPawn())
		{
            ADefaultPlayerState* PlayerState = Cast<ADefaultPlayerState>(PlayerController->GetPawn()->GetPlayerState());
            if(!PlayerState){ return; }

            if(PlayerState->KillCount == MaxKills)
            {
                HandleGameOver();
            } 
        }
    }
}

// void AFFAGameMode::RespawnPlayers() 
// {
    
// }

void AFFAGameMode::HandleGameOver() 
{
    SetActorTickEnabled(false);
    
    SetGameState(EFFAState::GameOver);

    for (FConstPlayerControllerIterator ControlledPlayer = GetWorld()->GetPlayerControllerIterator(); ControlledPlayer; ++ControlledPlayer)
	{
		APlayerController* PlayerController = ControlledPlayer->Get();
		if (PlayerController->GetPawn())
		{
            ADefaultPlayerState* PlayerState = Cast<ADefaultPlayerState>(PlayerController->GetPawn()->GetPlayerState());
            if(!PlayerState){ return; }

            if(PlayerState->KillCount == MaxKills)
            {
                Winner.Broadcast(PlayerController);
            }
            else
            {
                Loser.Broadcast(PlayerController);
            } 
        }
    }

    UE_LOG(LogTemp, Log, TEXT("GAME OVER! Max Kill Count Reached"));
}







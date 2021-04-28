// Fill out your copyright notice in the Description page of Project Settings.


#include "FFAGameMode.h"
#include "DefaultPlayerState.h"
#include "EngineUtils.h"
#include "PIXGameState.h"
#include "HealthComponent.h"
#include "PIXGameInstance.h"
#include "PlayerCharacter.h"
#include "PlayerHUD.h"
#include "TimerManager.h"

// Sets default values for this gamemode's properties.
AFFAGameMode::AFFAGameMode() 
{
    SetTeamsDelay = 3.5f;
    StartMatchDelay = 5.0f;

    GameStateClass = APIXGameState::StaticClass();
    PlayerStateClass = ADefaultPlayerState::StaticClass();
    DefaultPawnClass = APlayerCharacter::StaticClass();

    PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 1.0f;
}

// Called when the game starts or when spawned.
void AFFAGameMode::BeginPlay() 
{
    Super::BeginPlay();

    SetMatchState("WaitingForPlayers");

    SetActorTickEnabled(true);
}

// Handles starting game match.
void AFFAGameMode::StartMatch() 
{
    Super::StartMatch();
    
    UE_LOG(LogTemp, Warning, TEXT("Start Match"));
}

// Called every frame.
void AFFAGameMode::Tick(float DeltaSeconds) 
{
    Super::Tick(DeltaSeconds);

    // Wait for all players to load into map before triggering intro.
    if(NumTravellingPlayers == 0 && MatchState == "WaitingForPlayers")
    {
        CueGameIntro();
    }
    
    if(!(MatchState == "WaitingForPlayers" || MatchState == "GameModeIntro"))
    {
        CheckKillCount();
    }
}

/// TODO: Play test how well this works.
void AFFAGameMode::CueGameIntro() 
{
    SetMatchState("GameModeIntro");

    GetWorldTimerManager().SetTimer(TimerHandle_SetTeams, this, &AFFAGameMode::SetPlayerTeams, SetTeamsDelay, false);

    GetWorldTimerManager().SetTimer(TimerHandle_StartMatch, this, &AFFAGameMode::StartMatch, StartMatchDelay, false);

    SetMatchState("InProgress");
}

// Places each player on a unique team.
void AFFAGameMode::SetPlayerTeams() 
{
    int Team = 1;

    for (FConstPlayerControllerIterator ControlledPlayer = GetWorld()->GetPlayerControllerIterator(); ControlledPlayer; ++ControlledPlayer)
	{
        APlayerController* PlayerController = Cast<APlayerController>(ControlledPlayer->Get());
        UPIXGameInstance* GameInstance = Cast<UPIXGameInstance>(PlayerController->GetGameInstance());
        
        if(ensure(GameInstance))
        {
            GameInstance->PlayerTeam = Team;
            Team ++;
        }

        APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(PlayerController->GetPawn());
        if(PlayerCharacter)
        {
            UHealthComponent* HealthComp = Cast<UHealthComponent>(PlayerCharacter->GetComponentByClass(UHealthComponent::StaticClass()));
            
            if(ensure(HealthComp))
            {
                HealthComp->SetPlayerTeam();
            }
        }

    }
}

// Checks if max kill count reached.
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
                HandleGameOver(PlayerController);
            } 
        }
    }
}

// Handles game over.
void AFFAGameMode::HandleGameOver(APlayerController* Winner) 
{
    SetActorTickEnabled(false);

    EndMatch();

    for (FConstPlayerControllerIterator ControlledPlayer = GetWorld()->GetPlayerControllerIterator(); ControlledPlayer; ++ControlledPlayer)
	{
		APlayerController* PlayerController = ControlledPlayer->Get();
		if (PlayerController)
		{
            APlayerHUD* HUD = Cast<APlayerHUD>(PlayerController->GetHUD());
			if(!HUD){ return; }

            if(PlayerController == Winner)
            {
                HUD->Winner.Broadcast(PlayerController);
            }
            else
            {
                HUD->Loser.Broadcast(PlayerController);
            } 
        }
    }
}

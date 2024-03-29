// Fill out your copyright notice in the Description page of Project Settings.


#include "TDMGameMode.h"
#include "DefaultPlayerState.h"
#include "EngineUtils.h"
#include "PIXGameState.h"
#include "HealthComponent.h"
#include "PIXGameInstance.h"
#include "PlayerCharacter.h"
#include "PlayerHUD.h"
#include "TimerManager.h"

// Sets default values for this gamemode's properties.
ATDMGameMode::ATDMGameMode() 
{
    SetTeamsDelay = 3.5f;
    StartMatchDelay = 5.0f;

    GameStateClass = APIXGameState::StaticClass();
    PlayerStateClass = ADefaultPlayerState::StaticClass();
    DefaultPawnClass = APlayerCharacter::StaticClass();

    PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 1.0f;

    TeamCount_1 = 0;
    TeamCount_2 = 0;
}

// Called when the game starts or when spawned.
void ATDMGameMode::BeginPlay() 
{
    Super::BeginPlay();

    SetMatchState("WaitingForPlayers");

    SetActorTickEnabled(true);
}

// Handles starting game match.
void ATDMGameMode::StartMatch() 
{
    Super::StartMatch();

    UE_LOG(LogTemp, Warning, TEXT("Start Match"));
}

// Called every frame.
void ATDMGameMode::Tick(float DeltaSeconds) 
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

void ATDMGameMode::CueGameIntro() 
{
    SetMatchState("GameModeIntro");

    GetWorldTimerManager().SetTimer(TimerHandle_SetTeams, this, &ATDMGameMode::SetPlayerTeams, SetTeamsDelay, false);
    
    GetWorldTimerManager().SetTimer(TimerHandle_StartMatch, this, &ATDMGameMode::StartMatch, StartMatchDelay, false); 
    
    SetMatchState("InProgress");
}

// Randomly places each player on a team.
void ATDMGameMode::SetPlayerTeams() 
{
    for (FConstPlayerControllerIterator ControlledPlayer = GetWorld()->GetPlayerControllerIterator(); ControlledPlayer; ++ControlledPlayer)
	{
        APlayerController* PlayerController = Cast<APlayerController>(ControlledPlayer->Get());
        UPIXGameInstance* GameInstance = Cast<UPIXGameInstance>(PlayerController->GetGameInstance());
        
        if(ensure(GameInstance))
        {
            if(TeamCount_1 < (GetNumPlayers() / 2))
            {
                GameInstance->PlayerTeam = 1;
                TeamCount_1 ++;
            }
            else
            {
                GameInstance->PlayerTeam = 2;
                TeamCount_2 ++;
            }
        }

        APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(PlayerController->GetPawn());
        UHealthComponent* HealthComp = Cast<UHealthComponent>(PlayerCharacter->GetComponentByClass(UHealthComponent::StaticClass()));

        if(ensure(HealthComp))
        {
            HealthComp->SetPlayerTeam();
        }
    }
}

void ATDMGameMode::IncreaseTeamKillCount(int TeamNum) 
{
    if(TeamNum == 1)
    {
        Kills_T1 ++;
    }
    else
    {
        Kills_T2 ++;
    }
}

// Checks if max kill count reached.
void ATDMGameMode::CheckKillCount() 
{
    if(Kills_T1 == MaxKills)
    {
        HandleGameOver(1);
    }
    else if(Kills_T2 == MaxKills)
    {
        HandleGameOver(2);
    }
}


// Handles game over.
void ATDMGameMode::HandleGameOver(int WinningTeamNum) 
{
    UE_LOG(LogTemp, Warning, TEXT("GAME OVER!!!"));

    EndMatch();

    for(FConstPlayerControllerIterator ControlledPlayer = GetWorld()->GetPlayerControllerIterator(); ControlledPlayer; ++ControlledPlayer)
	{
		APlayerController* PlayerController = Cast<APlayerController>(ControlledPlayer->Get());
		if (PlayerController->GetPawn())
		{
            UPIXGameInstance* GameInstance = Cast<UPIXGameInstance>(PlayerController->GetPawn()->GetGameInstance());
            APlayerHUD* HUD = Cast<APlayerHUD>(PlayerController->GetHUD());

            if(GameInstance->PlayerTeam == WinningTeamNum)
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

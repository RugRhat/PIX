// Fill out your copyright notice in the Description page of Project Settings.


#include "HordeGameMode.h"
#include "BaseCharacter.h"
#include "Engine/Engine.h"
#include "EngineUtils.h"
#include "DefaultPlayerState.h"
#include "HealthComponent.h"
#include "PIXGameState.h"
#include "PIXGameInstance.h"
#include "PlayerCharacter.h"
#include "PlayerHUD.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

// Sets default values for this gamemode's properties.
AHordeGameMode::AHordeGameMode() 
{
    StartMatchDelay = 5.0f;
    TimeBetweenWaves = 5.0f;
    RespawnDelay = 5.0f;

    GameStateClass = APIXGameState::StaticClass();
    PlayerStateClass = ADefaultPlayerState::StaticClass();
    DefaultPawnClass = APlayerCharacter::StaticClass();

    PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 1.0f;
}

// Called when the game starts or when spawned.
void AHordeGameMode::BeginPlay() 
{
    Super::BeginPlay();

    UGameplayStatics::GetAllActorsOfClass(GetWorld(), SpawnPoints, SpawnLocations);

    SetMatchState("WaitingForPlayers");

    SetActorTickEnabled(true);
}

// Handles starting game match.
void AHordeGameMode::StartMatch() 
{
    Super::StartMatch();

    UE_LOG(LogTemp, Warning, TEXT("Start Match"));

    Wave = 1;
    EnemiesToSpawn = BaseNoOfEnemies;

    // StartWave();
}

// Called every frame.
void AHordeGameMode::Tick(float DeltaSeconds) 
{
    Super::Tick(DeltaSeconds);

    // Wait for all players to load into map before triggering intro.
    if(NumTravellingPlayers == 0 && MatchState == "WaitingForPlayers")
    {
        CueGameIntro();
    }
    
    if(!(MatchState == "WaitingForPlayers" || MatchState == "GameModeIntro"))
    {
        PlayersAlive();
        EnemiesAlive();
    }
}

// Changes match state to cue intro and starts countdown to start of match.
void AHordeGameMode::CueGameIntro() 
{
    SetMatchState("GameModeIntro");
    
    GetWorldTimerManager().SetTimer(TimerHandle_StartMatch, this, &AHordeGameMode::StartWave, StartMatchDelay, false);
}

void AHordeGameMode::StartWave() 
{
    SetMatchState("InProgress");

    GetWorldTimerManager().SetTimer(TimerHandle_EnemySpawner, this, &AHordeGameMode::EnemySpawner, 1.0f, true, 0.0f);
}

// Increases no. of enemies & wave no.
void AHordeGameMode::NewWave() 
{
    Wave ++;
    EnemiesToSpawn = BaseNoOfEnemies * Wave;

    GetWorldTimerManager().SetTimer(TimerHandle_StartNewWave, this, &AHordeGameMode::StartWave, TimeBetweenWaves, false);

    // Players respawn if at least 1 teammate survived wave.
    GetWorldTimerManager().SetTimer(TimerHandle_RespawnDeadPlayers, this, &AHordeGameMode::RespawnPlayers, RespawnDelay, false);  
}

void AHordeGameMode::EndWave() 
{
    GetWorldTimerManager().ClearTimer(TimerHandle_EnemySpawner);
}


// Checks if any players alive. If not, game ends. 
void AHordeGameMode::PlayersAlive() 
{
    for (FConstPlayerControllerIterator ControlledPlayer = GetWorld()->GetPlayerControllerIterator(); ControlledPlayer; ++ControlledPlayer)
	{
		APlayerController* PlayerController = ControlledPlayer->Get();
		if (PlayerController && PlayerController->GetPawn())
		{
			APawn* MyPawn = PlayerController->GetPawn();
			UHealthComponent* HealthComp = Cast<UHealthComponent>(MyPawn->GetComponentByClass(UHealthComponent::StaticClass()));
			if (ensure(HealthComp) && HealthComp->GetHealth() > 0.0f)
			{
				return;              // A player is still alive.
			}
		}
	}

	HandleGameOver();                 // No players alive.
}

// Checks if any enemies alive. If not, a new wave begins.
void AHordeGameMode::EnemiesAlive() 
{
    bool bWaitingForNewWave = GetWorldTimerManager().IsTimerActive(TimerHandle_StartNewWave);
    
    if(EnemiesToSpawn > 0 || bWaitingForNewWave)
    {
        return;
    }

    bool bEnemyAlive = false;

	for (TActorIterator<ABaseCharacter> Enemy(GetWorld()); Enemy; ++Enemy)          
	{
        if (!ensure(Enemy) || Enemy->IsPlayerControlled())
		{
            continue;
        }

        UHealthComponent* HealthComp = Cast<UHealthComponent>(Enemy->GetComponentByClass(UHealthComponent::StaticClass()));

        if (HealthComp && HealthComp->GetHealth() > 0.0f)
        {
            bEnemyAlive = true;
            break;

        }
	}

    // Wave complete if all enemies dead, so start new wave.
    if(!bEnemyAlive)
    { 
        SetMatchState("WaveComplete");
        
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Wave Complete!!"));
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Match State: %s"), *MatchState.ToString()));

        NewWave(); 
    }
}

void AHordeGameMode::IncreaseTeamKillCount() 
{
    TeamKills ++;
}

// Handles player respawning.
void AHordeGameMode::RespawnPlayers() 
{
    for (FConstPlayerControllerIterator ControlledPlayer = GetWorld()->GetPlayerControllerIterator(); ControlledPlayer; ++ControlledPlayer)
	{
		APlayerController* PlayerController = ControlledPlayer->Get();
		if (PlayerController && PlayerController->GetPawn() == nullptr)
		{
            RestartPlayer(PlayerController);  
            
            APlayerHUD* HUD = Cast<APlayerHUD>(PlayerController->GetHUD());
			if(!HUD){ return; }

			HUD->OnRespawned.Broadcast(PlayerController);
        }
    }
}

// Determines when to spawn enemies.
void AHordeGameMode::EnemySpawner() 
{
    SpawnEnemy();

    EnemiesToSpawn--;
    if(EnemiesToSpawn <= 0){ EndWave(); }
}

// Handles spawning of enemies.
void AHordeGameMode::SpawnEnemy() 
{
    int SpawnPoint = (rand() % SpawnLocations.Num()); 

    FVector SpawnPointLocation = SpawnLocations[SpawnPoint]->GetActorLocation();
    FRotator SpawnPointRotation = SpawnLocations[SpawnPoint]->GetActorRotation();

    GetWorld()->SpawnActor<ABaseCharacter>(EnemyCharacter, SpawnPointLocation, SpawnPointRotation);
}

// Handles game loss.
void AHordeGameMode::HandleGameOver() 
{
    UE_LOG(LogTemp, Log, TEXT("GAME OVER! All Players Died"));

    SetActorTickEnabled(false);

    EndWave();
    EndMatch();

    for (FConstPlayerControllerIterator ControlledPlayer = GetWorld()->GetPlayerControllerIterator(); ControlledPlayer; ++ControlledPlayer)
	{
		APlayerController* PlayerController = ControlledPlayer->Get();
		if (PlayerController)
		{
            APlayerHUD* HUD = Cast<APlayerHUD>(PlayerController->GetHUD());
			if(!HUD){ return; }

            HUD->Loser.Broadcast(PlayerController);
        }
    }
}

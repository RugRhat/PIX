// Fill out your copyright notice in the Description page of Project Settings.


#include "HordeGameMode.h"
#include "BaseCharacter.h"
#include "EngineUtils.h"
#include "DefaultPlayerState.h"
#include "HealthComponent.h"
#include "HordeGameState.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

// Sets default values for this gamemode's properties.
AHordeGameMode::AHordeGameMode() 
{
    StartGameDelay = 5.0f;
    TimeBetweenWaves = 2.0f;
    RespawnDelay = 5.0f;

    // Sets Default Player State as game mode's player state.
    PlayerStateClass = ADefaultPlayerState::StaticClass();

    PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 1.0f;
}

// Called when game starts, if gamemode selected.
void AHordeGameMode::BeginPlay() 
{
    Super::BeginPlay();

    UGameplayStatics::GetAllActorsOfClass(GetWorld(), SpawnPoints, SpawnLocations);

    SetGameState(EHordeState::WaitingToBegin);

    GetWorldTimerManager().SetTimer(TimerHandle_BeginGame, this, &AHordeGameMode::NewWave, StartGameDelay, false);
    // NewWave();
}

// Sets game state.
void AHordeGameMode::SetGameState(EHordeState NewState) 
{
    AHordeGameState* ThisGameState = GetGameState<AHordeGameState>();
	if (ensureAlways(ThisGameState))
	{
		ThisGameState->SetGameState(NewState);
	}
}

// Called every frame.
void AHordeGameMode::Tick(float DeltaSeconds) 
{
    Super::Tick(DeltaSeconds);

    PlayersAlive();
    EnemiesAlive();
}

void AHordeGameMode::StartWave() 
{
    Wave ++;
    EnemiesToSpawn = BaseNoOfEnemies * Wave;

    GetWorldTimerManager().SetTimer(TimerHandle_EnemySpawner, this, &AHordeGameMode::EnemySpawner, 1.0f, true, 0.0f);

    SetGameState(EHordeState::WaveStarted);
   
    SetActorTickEnabled(true);
}

void AHordeGameMode::NewWave() 
{
    GetWorldTimerManager().SetTimer(TimerHandle_StartNewWave, this, &AHordeGameMode::StartWave, TimeBetweenWaves, false);

    // Players respawn if at least 1 teammate survived wave.
    GetWorldTimerManager().SetTimer(TimerHandle_RespawnDeadPlayers, this, &AHordeGameMode::RespawnPlayers, RespawnDelay, false);  
}

void AHordeGameMode::EndWave() 
{
    GetWorldTimerManager().ClearTimer(TimerHandle_EnemySpawner);
}

// Handles player health regeneration if any players alive. 
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
                // Regen health if damaged
                if(HealthComp->GetHealth() < HealthComp->DefaultHealth)
                {
                    HealthComp->Heal(2.0f);
                }
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
        SetGameState(EHordeState::WaveComplete);

        NewWave(); 
    }
}

// Handles spawning of enemies.
void AHordeGameMode::SpawnEnemy() 
{
    int SpawnPoint = (rand() % SpawnLocations.Num()); 

    FVector SpawnPointLocation = SpawnLocations[SpawnPoint]->GetActorLocation();
    FRotator SpawnPointRotation = SpawnLocations[SpawnPoint]->GetActorRotation();

    GetWorld()->SpawnActor<ABaseCharacter>(EnemyCharacter, SpawnPointLocation, SpawnPointRotation);
}

// Handles player respawning (actual respawn called in blueprint).
void AHordeGameMode::RespawnPlayers() 
{
    for (FConstPlayerControllerIterator ControlledPlayer = GetWorld()->GetPlayerControllerIterator(); ControlledPlayer; ++ControlledPlayer)
	{
		APlayerController* PlayerController = ControlledPlayer->Get();
		if (PlayerController && PlayerController->GetPawn() == nullptr)
		{
            RestartPlayer(PlayerController);  
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

// Handles game loss.
void AHordeGameMode::HandleGameOver() 
{
    SetActorTickEnabled(false);

    EndWave();

    SetGameState(EHordeState::GameOver);

    UE_LOG(LogTemp, Log, TEXT("GAME OVER! All Players Died"));

    // TODO: Implement UI endgame handling.
}

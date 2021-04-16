// Fill out your copyright notice in the Description page of Project Settings.


#include "HordeAI.h"
#include "BaseCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"


// Called when the game starts or when spawned.
void AHordeAI::BeginPlay() 
{
    Super::BeginPlay();
    
    if(AIBehavior != nullptr)
    {
        RunBehaviorTree(AIBehavior);
    }
}

// Called every frame.
void AHordeAI::Tick(float DeltaSeconds) 
{
    Super::Tick(DeltaSeconds);

    ABaseCharacter* ControlledCharacter = Cast<ABaseCharacter>(GetPawn());
    if(ControlledCharacter == nullptr){ return; }

    APlayerCharacter* TargetPlayer = nullptr;
    
    for (FConstPlayerControllerIterator ControlledPlayer = GetWorld()->GetPlayerControllerIterator(); ControlledPlayer; ++ControlledPlayer)
	{
		APlayerController* PlayerController = ControlledPlayer->Get();
		if (PlayerController->GetPawn())
		{
            APawn* PotentialTarget = PlayerController->GetPawn();

            // Finds closest player.
            FVector PlayerDistance = ControlledCharacter->GetActorLocation() - PotentialTarget->GetActorLocation();
            float DistanceToPlayer = PlayerDistance.Size();

            if(DistanceToPlayer < AttackRange)
            {
                TargetPlayer = Cast<APlayerCharacter>(PotentialTarget);
                AttackRange = DistanceToPlayer;
            }
        }
    }

    if(TargetPlayer)
    {
        if(!TargetPlayer->bDead)
        {
            GetBlackboardComponent()->SetValueAsObject(TEXT("NearestPlayer"), TargetPlayer);
        }
        else
        {
            GetBlackboardComponent()->ClearValue(TEXT("NearestPlayer"));
            AttackRange = 100000.0f;
        }
    }
}

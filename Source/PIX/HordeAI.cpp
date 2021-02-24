// Fill out your copyright notice in the Description page of Project Settings.


#include "HordeAI.h"
#include "BaseCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
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

    // Creates an array of all BP_Players.
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), PotentialPlayers, Players);

    if(Players.Num() == 0)
    {
        return;         // TODO:Implement game end/loss.
    }

    ABaseCharacter* ControlledCharacter = Cast<ABaseCharacter>(GetPawn());
    if(ControlledCharacter == nullptr){ return; }

    APlayerCharacter* TargetPlayer = Cast<APlayerCharacter>(Players[0]);

    for(AActor* PotentialTarget : Players)
    {
        // Finds closest player.
        FVector PlayerDistance = ControlledCharacter->GetActorLocation() - PotentialTarget->GetActorLocation();
        float DistanceToPlayer = PlayerDistance.Size();

        if(DistanceToPlayer < AttackRange)
        {
            TargetPlayer = Cast<APlayerCharacter>(PotentialTarget);
            AttackRange = DistanceToPlayer;
        }
    }

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

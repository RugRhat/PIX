// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "FFAGameMode.h"
#include "HordeGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "PIXGameInstance.h"
#include "PIXGameState.h"
#include "PlayerHUD.h"
#include "TDMGameMode.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	DefaultHealth = 100;
	bIsDead = false;

	TeamNum = 9;		// Default is set to 9 for Horde AI.

	SetIsReplicatedByDefault(true);
}

// Controls which properties are replicated.
void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UHealthComponent, Health);
}

// Called when the game starts or when owning character is spawned.
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// Only create dynamic damage event if server.
	if (GetOwnerRole() == ROLE_Authority)
	{
		AActor* MyOwner = GetOwner();
		if (MyOwner)
		{
			MyOwner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::HandleTakeAnyDamage);
		}
	}

	Health = DefaultHealth;
}

// Assigns player's team to TeamNum.
void UHealthComponent::SetPlayerTeam() 
{
	APlayerController* PlayerController = Cast<APlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	
	if(PlayerController->GetPawn())
	{
		UPIXGameInstance* GameInstance = Cast<UPIXGameInstance>(PlayerController->GetPawn()->GetGameInstance());

		TeamNum = GameInstance->PlayerTeam;
	}
}

// Handles what happens when character health is replicated.
void UHealthComponent::OnRep_Health(float OldHealth) 
{
	float Damage = Health - OldHealth;

	OnHealthChanged.Broadcast(this, Health, Damage, nullptr, nullptr, nullptr);
}

// Called when the character sustains any damage.
void UHealthComponent::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser) 
{
	if (Damage <= 0.0f || bIsDead){ return; }

	if (DamageCauser != DamagedActor && IsFriendly(DamagedActor, DamageCauser)){ return; }

	// Update health clamped
	Health = FMath::Clamp(Health - Damage, 0.0f, DefaultHealth);

	bIsDead = Health <= 0.0f;

	OnHealthChanged.Broadcast(this, Health, Damage, DamageType, InstigatedBy, DamageCauser);

	if (bIsDead)
	{
		UE_LOG(LogTemp, Log, TEXT("I'M DEAD!!!"));

		APawn* MyPawn = Cast<APawn>(GetOwner());
		if(!MyPawn){ return ;}

		HandleGMDeath(MyPawn, InstigatedBy);

		if(MyPawn->IsPlayerControlled())
		{
			APIXGameState* GameState = Cast<APIXGameState>(GetWorld()->GetGameState());
			if(!GameState){ return; }
			
			if(GameState->GetMatchState() != "WaitingPostMatch")
			{
				APlayerHUD* HUD = Cast<APlayerHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
				if(!HUD){ return; }

				HUD->OnPlayerDied.Broadcast(MyPawn, MyPawn->GetController());
			}
		}
	}
}

// Determines if given characters are on the same team (friendly).
bool UHealthComponent::IsFriendly(AActor* ActorA, AActor* ActorB) 
{
	// Assume Friendly
	if (ActorA == nullptr || ActorB == nullptr){ return true; }

	UHealthComponent* HealthCompA = Cast<UHealthComponent>(ActorA->GetComponentByClass(UHealthComponent::StaticClass()));
	UHealthComponent* HealthCompB = Cast<UHealthComponent>(ActorB->GetComponentByClass(UHealthComponent::StaticClass()));

	// Assume friendly
	if (HealthCompA == nullptr || HealthCompB == nullptr){ return true; }

	return HealthCompA->TeamNum == HealthCompB->TeamNum;
}

// Handles character healing.
void UHealthComponent::Heal(float HealAmount) 
{
	if (HealAmount <= 0.0f || Health <= 0.0f){ return; }

	// Ensures that health is never less than 0 or more than Default Health.
	Health = FMath::Clamp(Health + HealAmount, 0.0f, DefaultHealth);	

	OnHealthChanged.Broadcast(this, Health, -HealAmount, nullptr, nullptr, nullptr);
}

// Returns character's health.
float UHealthComponent::GetHealth() const
{
	return Health;
}

// Handles triggering actor killed event on active gamemode.
void UHealthComponent::HandleGMDeath(APawn* Victim, class AController* KillerController) 
{
	class UPIXGameInstance* GameInstanceRef = Cast<UPIXGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if(GameInstanceRef)
	{
	 	if(GameInstanceRef->GetGameModeID() == "Horde")
	 	{
	 		AHordeGameMode* GM = GetWorld()->GetAuthGameMode<AHordeGameMode>();
			if(GM)
			{ 
				UE_LOG(LogTemp, Warning, TEXT("Game Mode: HORDE"));
				GM->OnActorKilled.Broadcast(Victim, KillerController); 
			}
	 	}
		else if(GameInstanceRef->GetGameModeID() == "TDM")
		{
			ATDMGameMode* GM = GetWorld()->GetAuthGameMode<ATDMGameMode>();
			if(GM)
			{
				UE_LOG(LogTemp, Warning, TEXT("Game Mode: TDM"));
				GM->OnActorKilled.Broadcast(Victim, KillerController);
			}
		}
	 	else //if(GameInstanceRef->GetGameModeID() == "FFA")		// Defaults to FFA (FOR DEV ONLY!!!!) 
	 	{
			AFFAGameMode* GM = GetWorld()->GetAuthGameMode<AFFAGameMode>();
			if(GM)
			{ 
				UE_LOG(LogTemp, Warning, TEXT("Game Mode: FFA"));
				GM->OnActorKilled.Broadcast(Victim, KillerController); 
			}
	 	}
	}
}

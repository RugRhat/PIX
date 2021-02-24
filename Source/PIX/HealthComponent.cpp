// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	DefaultHealth = 100;
	bIsDead = false;

	TeamNum = 9;		// Nine is default for Horde AI characters.

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

		// ASGameMode* GM = Cast<ASGameMode>(GetWorld()->GetAuthGameMode());
		// if (GM)
		// {
		// 	GM->OnActorKilled.Broadcast(GetOwner(), DamageCauser, InstigatedBy);
		// }
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

	Health = FMath::Clamp(Health + HealAmount, 0.0f, DefaultHealth);

	OnHealthChanged.Broadcast(this, Health, -HealAmount, nullptr, nullptr, nullptr);
}

// Returns character's health.
float UHealthComponent::GetHealth() const
{
	return Health;
}



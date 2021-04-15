// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "HealthComponent.h"
#include "Net/UnrealNetwork.h"
#include "Weapon.h"

// Sets default values.
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComp"));
}

// Controls which properties are replicated.
void ABaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABaseCharacter, Weapon);
	DOREPLIFETIME(ABaseCharacter, bDead);
	DOREPLIFETIME(ABaseCharacter, bReloading);
}

// Called when the game starts or when character is spawned.
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	HealthComponent->OnHealthChanged.AddDynamic(this, &ABaseCharacter::OnHealthChanged);
	
	// "Has authority" refers to server. Server controls spawning of selected weapon.
	if(HasAuthority())
	{
		SpawnWeapon();
	}
}

void ABaseCharacter::SpawnWeapon() 
{
	// Ensures that weapon spawns even if colliding with character mesh.
	FActorSpawnParameters SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	Weapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass, SpawnParam);
	if(!ensure(Weapon != nullptr)){return;}

	Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
	Weapon->SetOwner(this);

}

// Called when fire button pressed or when AI decides to shoot.
void ABaseCharacter::UseWeapon() 
{
	Weapon->PullTrigger();
}

// Called to trigger weapon reloading and reload animation.
void ABaseCharacter::Reload() 
{
	// If client calls Reload RPC is called to reload on server.
	if(!HasAuthority())
	{
		Server_Reload();
	}

	bReloading = true;

	Weapon->Reload();
}

// Returns ammo count so AI knows when to reload.
float ABaseCharacter::GetAmmoCount() 
{
	return Weapon->AmmoCount;
}

// Called when character health is changed.
void ABaseCharacter::OnHealthChanged(UHealthComponent* OwningHealthComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser) 
{
	if (Health <= 0.0f && !bDead)
	{
		// Die!
		bDead = true;

		GetMovementComponent()->StopMovementImmediately();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		// Stops AI from shooting at player if AI is dead.
		if(!IsPlayerControlled())
		{
			DetachFromControllerPendingDestroy();
		}

		// Ensures that player's weapon also disapears from world on death.
		Weapon->SetLifeSpan(2.25f);
		SetLifeSpan(2.25f);
	}
}


// Reload RPC (Remote Proceedure Call).
void ABaseCharacter::Server_Reload_Implementation() 
{
	Reload();
}

bool ABaseCharacter::Server_Reload_Validate() 
{
	return true;
}

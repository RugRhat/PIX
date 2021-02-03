// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Particles/ParticleSystem.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);

	SetReplicates(true);

	// How often network updates.
	NetUpdateFrequency = 66.0f;
	MinNetUpdateFrequency = 33.0f;
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
}

// Weapon firing uses hitscan.
void AWeapon::PullTrigger() 
{
	// If client calls pull trigger RPC is called to pull trigger on server.
	if(!GetOwner()->HasAuthority())
	{
		Server_PullTrigger();
	}
	
	FHitResult Hit;
	FVector ShotDirection;
	bool bHitSomething = GunTrace(Hit, ShotDirection);

	if(bHitSomething)
	{
		// For Debugging:
		UE_LOG(LogTemp, Warning, TEXT("HIT!!!"));
		
		AActor* HitActor = Hit.GetActor();
		if(HitActor != nullptr){
			AController* OwnerController = GetOwnerController();
			FPointDamageEvent DamageEvent(Damage, Hit, ShotDirection, nullptr);
			HitActor->TakeDamage(Damage, DamageEvent, OwnerController, this);
		}

		ImpactEffects(Hit.Location, ShotDirection.Rotation());
	}

	ShootEffects();
}

// Handles hitscan and returns whether hitscan was successful.
bool AWeapon::GunTrace(FHitResult& Hit, FVector& ShotDirection) 
{
	AController* OwnerController = GetOwnerController();
	if(!ensure(OwnerController != nullptr)){return false;}

	FVector Location;
	FRotator Rotation;
	OwnerController->GetPlayerViewPoint(Location, Rotation);

	ShotDirection = -Rotation.Vector();

	FVector End = Location + Rotation.Vector() * MaxRange;
	
	// Stop actors from shooting themselves.
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());

	return GetWorld()->LineTraceSingleByChannel(Hit, Location, End, ECollisionChannel::ECC_GameTraceChannel1, Params);
}

// Plays effects when weapon is shot.			TODO: Add effects.
void AWeapon::ShootEffects() 
{
	// UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("MuzzleFlashSocket"));
	// UGameplayStatics::SpawnSoundAttached(MuzzleSound, Mesh, TEXT("MuzzleSound"));
}

// Plays effects on impact.						TODO: Add effects.
void AWeapon::ImpactEffects(FVector Location, FRotator Rotation) 
{
	// UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Location, Rotation);
	// UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, Location);
}

AController* AWeapon::GetOwnerController() const
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if(!ensure(OwnerPawn != nullptr)){return nullptr;}

	return OwnerPawn->GetController();
}

// Pull trigger RPC on server.
void AWeapon::Server_PullTrigger_Implementation() 
{
	PullTrigger();
}

// Pull trigger RPC validation to protect against cheating.
bool AWeapon::Server_PullTrigger_Validate() 
{
	return true;
}


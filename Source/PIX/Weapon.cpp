// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/Material.h"
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

// Controls which properties are replicated.
void AWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWeapon, AmmoCount);
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	AmmoCount = WeaponClipSize;
}

void AWeapon::Tick(float DeltaSeconds) 
{
	Super::Tick(DeltaSeconds);

	if(GetOwner()== nullptr)
	{
		SetLifeSpan(0.1f);
	}

}

// Handles weapon firing using line trace.
void AWeapon::PullTrigger() 
{
	// If client calls pull trigger RPC is called to pull trigger on server.
	if(!GetOwner()->HasAuthority())
	{
		Server_PullTrigger();
	}
	
	if(HasAmmo())
	{
		FHitResult Hit;
		FVector ShotDirection;
		bool bHitSomething = GunTrace(Hit, ShotDirection);

		if(bHitSomething)
		{
			AActor* HitActor = Hit.GetActor();
			if(HitActor != nullptr){
				UGameplayStatics::ApplyPointDamage(HitActor, Damage, ShotDirection, Hit, GetOwner()->GetInstigatorController(), GetOwner(), DamageType);
			}

			ImpactEffects(Hit.Location, ShotDirection.Rotation());
		}

		AmmoCount --;
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

void AWeapon::OnRep_AmmoCount() 
{
	// ShootEffects();
}

// Plays effects when weapon is shot.			
void AWeapon::ShootEffects() 
{
	if(HasAmmo())
	{
		UGameplayStatics::SpawnSoundAttached(Gunshot, Mesh, TEXT("MuzzleSound"));
	}
	else
	{
		UGameplayStatics::SpawnSoundAttached(Dryfire, Mesh, TEXT("MuzzleSound"));
	}
}

///TODO: Add effects.
// Plays effects on impact.						
void AWeapon::ImpactEffects(FVector Location, FRotator Rotation) 
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, Location);
}

// Returns whether or not weapon has ammo.
bool AWeapon::HasAmmo() 
{
	if(AmmoCount == 0)
	{
		return false;
	}

	return true; 
}

void AWeapon::Reload() 
{
	UE_LOG(LogTemp, Warning, TEXT("RELOADING!"));
	
	UGameplayStatics::SpawnSoundAttached(ReloadSound, Mesh, TEXT("MuzzleSound"));

	AmmoCount = WeaponClipSize;
}

// Returns controller which "owns" weapon.
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


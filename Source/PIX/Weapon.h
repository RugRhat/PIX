// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponFired);

UCLASS()
class PIX_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

	// Handles weapon firing using line trace.
	void PullTrigger();

	void Reload();

	UPROPERTY(ReplicatedUsing = OnRep_AmmoCount, BlueprintReadOnly, Category = "Weapon")
	float AmmoCount; 


protected:
	// Handles linetrace and returns whether trace was successful.
	bool GunTrace(FHitResult& Hit, FVector& ShotDirection);

	// Plays effects when weapon is shot.
	void ShootEffects();

	// Plays effects on impact.
	void ImpactEffects(FVector Location, FRotator Rotation);

	// Returns whether or not weapon has ammo.
	bool HasAmmo();

	// Returns controller which "owns" weapon.
	AController* GetOwnerController() const;

	UPROPERTY(VisibleAnywhere)
	class USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	class USkeletalMeshComponent* Mesh;

	// Dynamic event that is assigned through blueprint.
    UPROPERTY(BlueprintAssignable, Category = "Weapon")
    FOnWeaponFired WeaponFired;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(EditDefaultsOnly)
	float WeaponClipSize = 30.f;

	UPROPERTY(EditAnywhere)
	class USoundBase* Gunshot;

	UPROPERTY(EditAnywhere)
	class USoundBase* Dryfire;

	UPROPERTY(EditAnywhere)
	class USoundBase* ImpactSound; 	

	UPROPERTY(EditAnywhere)
	class USoundBase* ReloadSound;		

	UPROPERTY(EditAnywhere)
	float MaxRange = 1000.f;

	UPROPERTY(EditAnywhere)
	float Damage = 10.f;

	// Handles what happens when ammo count is replicated.
	UFUNCTION()
	void OnRep_AmmoCount();

	// Pull trigger RPC (Remote Proceedure Call).
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_PullTrigger();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;
};

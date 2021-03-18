// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class PIX_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

	void PullTrigger();

	void Reload();

	UPROPERTY(ReplicatedUsing = OnRep_AmmoCount, BlueprintReadOnly, Category = "Weapon")
	float AmmoCount; 

protected:
	bool GunTrace(FHitResult& Hit, FVector& ShotDirection);

	void ShootEffects();
	void ImpactEffects(FVector Location, FRotator Rotation);

	bool HasAmmo();

	AController* GetOwnerController() const;

	UPROPERTY(VisibleAnywhere)
	class USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	class USkeletalMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(EditDefaultsOnly)
	float WeaponClipSize = 30.f;

	UPROPERTY(EditAnywhere)
	class USoundBase* Gunshot;

	UPROPERTY(EditAnywhere)
	class USoundBase* Dryfire;

	UPROPERTY(EditAnywhere)
	class USoundBase* ImpactSound; 			// Using dry fire sound.

	UPROPERTY(EditAnywhere)
	class UMaterial* MuzzleFlash;
	
	UPROPERTY(EditAnywhere)
	class UMaterial* ImpactEffect;

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
};

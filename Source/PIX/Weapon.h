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

private:
	bool GunTrace(FHitResult& Hit, FVector& ShotDirection);

	void ShootEffects();
	void ImpactEffects(FVector Location, FRotator Rotation);

	AController* GetOwnerController() const;

	UPROPERTY(VisibleAnywhere)
	class USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	class USkeletalMeshComponent* Mesh;

	// UPROPERTY(EditAnywhere)
	// UParticleSystem* MuzzleFlash;

	// UPROPERTY(EditAnywhere)
	// USoundBase* MuzzleSound;

	// UPROPERTY(EditAnywhere)
	// UParticleSystem* ImpactEffect;

	// UPROPERTY(EditAnywhere)
	// USoundBase* ImpactSound;

	UPROPERTY(EditAnywhere)
	float MaxRange = 1000.f;

	UPROPERTY(EditAnywhere)
	float Damage = 10.f;

	// Pull trigger RPC (Remote Proceedure Call).
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerPullTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

class AWeapon;

UCLASS()
class PIX_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties.
	ABaseCharacter();

	void UseWeapon();

	UPROPERTY(Replicated, BlueprintReadOnly)
	bool bDead;

	// UPROPERTY(Replicated, BlueprintReadOnly)
	UPROPERTY(Replicated, BlueprintReadWrite)
	bool bReloading;

	void Reload();

	// Returns ammo count so AI knows when to reload.
	float GetAmmoCount();

protected:
	// Called when the game starts or when spawned.
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	class UHealthComponent* HealthComponent;

	UPROPERTY(Replicated)
	AWeapon* Weapon;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AWeapon> WeaponClass;

	// Called when character health is changed.
	UFUNCTION()
	void OnHealthChanged(UHealthComponent* OwningHealthComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	// Start Aiming RPC (Remote Proceedure Call).
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Reload();
};

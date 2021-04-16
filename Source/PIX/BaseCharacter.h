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

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Gameplay")
	bool bDead;

	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Gameplay")
	bool bReloading;

	// Called when fire button pressed or when AI decides to shoot.
	void UseWeapon();

	// Called to trigger weapon reloading.
	void Reload();

	// Returns ammo count so AI knows when to reload.
	float GetAmmoCount();
	
protected:
	// Called when the game starts or when spawned.
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Health")
	class UHealthComponent* HealthComponent;

	UPROPERTY(Replicated)
	AWeapon* Weapon;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<AWeapon> WeaponClass;

	// Called when character health is changed.
	UFUNCTION()
	void OnHealthChanged(UHealthComponent* OwningHealthComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	// Reload RPC (Remote Proceedure Call).
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Reload();

	void SpawnWeapon();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PIXPlayer.generated.h"

class AWeapon;

UCLASS()
class PIX_API APIXPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APIXPlayer();

	UFUNCTION(BlueprintPure)
	bool IsAiming() const;

	UFUNCTION(BlueprintPure)
	bool IsDead() const;

	// Returns health percentage for health bar.
	UFUNCTION(BlueprintPure)
	float GetHealthPercent() const;

	void UseWeapon();

private:
	void MoveForward(float AxisValue);	
	void MoveRight(float AxisValue);
	void LookUpRate(float AxisValue);
	void LookRightRate(float AxisValue);
	
	void Jump();
	void PlayerCrouch();
	// void Sprint();		TODO: Implement sprinting.
	
	void StartAiming();
	void StopAiming();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere)
	float RotationRate = 70.f;

	UPROPERTY(EditAnywhere)
	float SprintSpeed = 10.f;

	UPROPERTY(EditDefaultsOnly)
	float DefaultHealth = 100.f;

	UPROPERTY(VisibleAnywhere)
	float Health;

	UPROPERTY(Replicated)
	AWeapon* Weapon;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AWeapon> WeaponClass;

	UPROPERTY(Replicated)
	bool bDead;

	UPROPERTY(Replicated)
	bool bAiming;

	bool bCrouching;

protected:
	// Called when the game starts or when spawned.
	virtual void BeginPlay() override;

	// Called every frame.
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input.
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Called when damage is taken.
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const &DamageEvent, class AController *EventInstigator, AActor *DamageCauser) override;
};

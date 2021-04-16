// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "PlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class PIX_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()
	
public:
	// Sets default values.
	APlayerCharacter();

	// Returns health percentage for health bar.
	UFUNCTION(BlueprintPure)
	float GetHealthPercent() const;

	class UCharacterMovementComponent* MovementComponent;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	class UAnimMontage* AimingReloadMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	class UAnimMontage* ReloadMontage;

	UPROPERTY(EditAnywhere)
	float RotationRate = 70.f;

	UPROPERTY(EditAnywhere)
	float RegularSpeed = 400.f;

	UPROPERTY(EditAnywhere)
	float AimingSpeed = 200.f;

	UPROPERTY(Replicated, BlueprintReadOnly)
	bool bAiming;

	// Start Aiming RPC (Remote Proceedure Call).
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_StartAiming();

	// Stop Aiming RPC (Remote Proceedure Call).
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_StopAiming();

	// Called when by lobby menu to change player's weapon.
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void ChangeWeapon(TSubclassOf<AWeapon> NewWeapon);

	void MoveForward(float AxisValue);	
	void MoveRight(float AxisValue);
	void LookUpRate(float AxisValue);
	void LookRightRate(float AxisValue);
	
	void Jump();
	void PlayerCrouch();

	/// TODO: Implement sprinting.
	// void Sprint();		
	
	void StartAiming();
	void StopAiming();

	// Called to trigger reload animation.
	void Reload();

	// Called when the game starts or when spawned.
	virtual void BeginPlay() override;

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	// Called to bind functionality to input.
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Used to control whether character crouch / uncrouch is called.
	bool bCrouching;

	// Pointer to player's health component.
	class UHealthComponent* HealthComp;

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PIXPlayer.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class PIX_API APIXPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APIXPlayer();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere)
	float RotationRate = 70.f;

	void Forward(float AxisValue);
	void Right(float AxisValue);

	void LookUp(float AxisValue);
	void LookRight(float AxisValue);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};

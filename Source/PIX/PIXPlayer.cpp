// Fill out your copyright notice in the Description page of Project Settings.


#include "PIXPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
APIXPlayer::APIXPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

// Called when the game starts or when spawned
void APIXPlayer::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APIXPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void APIXPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("Forward"), this, &APIXPlayer::Forward);
	PlayerInputComponent->BindAxis(TEXT("Right"), this, &APIXPlayer::Right);
	PlayerInputComponent->BindAxis(TEXT("Look Up"), this, &APIXPlayer::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Look Right"), this, &APIXPlayer::LookRight);
}

void APIXPlayer::Forward(float AxisValue) 
{
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

void APIXPlayer::Right(float AxisValue) 
{
	AddMovementInput(GetActorRightVector() * AxisValue);
}

void APIXPlayer::LookUp(float AxisValue) 
{
	AddControllerPitchInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

void APIXPlayer::LookRight(float AxisValue) 
{
	AddControllerYawInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values.
APlayerCharacter::APlayerCharacter() 
{
    // Creates spring arm which controls camera movement.
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);

	// Creates & attaches camera to spring arm.
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	// Allows character to crouch.
	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;

	MovementComponent = GetCharacterMovement();
}

// Controls which properties are replicated.
void APlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayerCharacter, bAiming);
}

// Called when the game starts or when spawned.
void APlayerCharacter::BeginPlay() 
{
    Super::BeginPlay();

	if(MovementComponent)
	{
		MovementComponent->MaxWalkSpeed = RegularSpeed;
	}
	
    bAiming = false;
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) 
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &APlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUpRate"), this, &APlayerCharacter::LookUpRate);
	PlayerInputComponent->BindAxis(TEXT("LookRightRate"), this, &APlayerCharacter::LookRightRate);
	
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Crouch"), EInputEvent::IE_Pressed, this, &APlayerCharacter::PlayerCrouch);
	// PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Pressed, this, &APIXPlayer::Jump);		TODO: Implement sprinting.

	PlayerInputComponent->BindAction(TEXT("Shoot"), EInputEvent::IE_Pressed, this, &APlayerCharacter::UseWeapon);
	PlayerInputComponent->BindAction(TEXT("Aim"), EInputEvent::IE_Pressed, this, &APlayerCharacter::StartAiming);
	PlayerInputComponent->BindAction(TEXT("Aim"), EInputEvent::IE_Released, this, &APlayerCharacter::StopAiming);
}

void APlayerCharacter::MoveForward(float AxisValue) 
{
    AddMovementInput(GetActorForwardVector() * AxisValue);
}

void APlayerCharacter::MoveRight(float AxisValue) 
{
    AddMovementInput(GetActorRightVector() * AxisValue);
}

// Controls the camera movement rate for controller's y axis.
void APlayerCharacter::LookUpRate(float AxisValue) 
{
    AddControllerPitchInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

// Controls the camera movement rate for controller's x axis.
void APlayerCharacter::LookRightRate(float AxisValue) 
{
    AddControllerYawInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::Jump() 
{
    if(bCrouching)
	{
		PlayerCrouch();
	} 
    else
	{
		ACharacter::Jump();
	}
}

// Crouches/uncrouches character when crouch button pressed.
void APlayerCharacter::PlayerCrouch() 
{
    if(bCrouching)
	{
		UnCrouch();
		bCrouching = false;
	} else
	{
		Crouch();
		bCrouching = true;
	}
}

// Pulls camera in when aiming.
void APlayerCharacter::StartAiming() 
{
    // If client calls Start Aiming RPC is called to start aiming on server.
	if(!HasAuthority())
	{
		Server_StartAiming();
	}

	bAiming = true;
	SpringArm->TargetArmLength = 125.f;

	if(MovementComponent)
	{
		MovementComponent->MaxWalkSpeed = AimingSpeed;
	}
}

// Returns camera to default position when not aiming.
void APlayerCharacter::StopAiming() 
{
    // If client calls Stop Aiming RPC is called to stop aiming on server.
	if(!HasAuthority())
	{
		Server_StopAiming();
	}

	bAiming = false;
	SpringArm->TargetArmLength = 250.f;

	if(MovementComponent)
	{
		MovementComponent->MaxWalkSpeed = RegularSpeed;
	}
}

// Start Aiming RPC on server.
void APlayerCharacter::Server_StartAiming_Implementation() 
{
    StartAiming();
}

bool APlayerCharacter::Server_StartAiming_Validate() 
{
    return true;
}

// Stop Aiming RPC on server.
void APlayerCharacter::Server_StopAiming_Implementation() 
{
    StopAiming();
}

bool APlayerCharacter::Server_StopAiming_Validate() 
{
    return true;
}

float APlayerCharacter::GetHealthPercent() const
{
    return 0;       // TODO: implement grabbing health percentage.
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "PIXPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Weapon.h"
#include "Net/UnrealNetwork.h"

// Sets default values.
APIXPlayer::APIXPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Creates spring arm which controls camera movement.
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);

	// Creates & attaches camera to spring arm.
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	// Allows character to crouch.
	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;
}

// Controls which properties are replicated.
void APIXPlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APIXPlayer, Weapon);
	DOREPLIFETIME(APIXPlayer, bDead);

	// TODO: Figure out animation replication*.
	// DOREPLIFETIME(APIXPlayer, bAiming);
	// DOREPLIFETIME(APIXPlayer, bCrouching);
}

// Called when the game starts or when spawned.
void APIXPlayer::BeginPlay()
{
	Super::BeginPlay();

	Health = DefaultHealth;

	// "Has authority" refers to server. Server controls spawning of selected weapon.
	if(HasAuthority())
	{
		// Ensures that weapon spawns even if colliding with character mesh.
		FActorSpawnParameters SpawnParam;
		SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		
		Weapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass, SpawnParam);
		if(!ensure(Weapon != nullptr)){return;}
		
		Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("Hand_R"));
		Weapon->SetOwner(this);
	}

	bAiming = false;
	bCrouching = false;
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

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &APIXPlayer::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &APIXPlayer::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUpRate"), this, &APIXPlayer::LookUpRate);
	PlayerInputComponent->BindAxis(TEXT("LookRightRate"), this, &APIXPlayer::LookRightRate);
	
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Crouch"), EInputEvent::IE_Pressed, this, &APIXPlayer::PlayerCrouch);
	// PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Pressed, this, &APIXPlayer::Jump);		TODO: Implement sprinting.

	PlayerInputComponent->BindAction(TEXT("Shoot"), EInputEvent::IE_Pressed, this, &APIXPlayer::UseWeapon);
	PlayerInputComponent->BindAction(TEXT("Aim"), EInputEvent::IE_Pressed, this, &APIXPlayer::StartAiming);
	PlayerInputComponent->BindAction(TEXT("Aim"), EInputEvent::IE_Released, this, &APIXPlayer::StopAiming);
}

void APIXPlayer::MoveForward(float AxisValue) 
{
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

void APIXPlayer::MoveRight(float AxisValue) 
{
	AddMovementInput(GetActorRightVector() * AxisValue);
}

// Controls the camera movement rate for controller's y axis.
void APIXPlayer::LookUpRate(float AxisValue) 
{
	AddControllerPitchInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

// Controls the camera movement rate for controller's x axis.
void APIXPlayer::LookRightRate(float AxisValue) 
{
	AddControllerYawInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

void APIXPlayer::Jump() 
{
	if(bCrouching)
	{
		bCrouching = false;
	} else
	{
		ACharacter::Jump();
	}
}

// Crouches/uncrouches character when crouch button pressed.
void APIXPlayer::PlayerCrouch() 
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
void APIXPlayer::StartAiming() 
{
	bAiming = true;
	SpringArm->TargetArmLength = 125.f;
}

// Returns camera to default position when not aiming.
void APIXPlayer::StopAiming() 
{
	bAiming = false;
	SpringArm->TargetArmLength = 250.f;
}

// void APIXPlayer::Sprint() 
// {
// 	TODO: Implement sprinting.
// }

void APIXPlayer::UseWeapon() 
{
	// For debugging:
	UE_LOG(LogTemp, Warning, TEXT("SHOOT!!"));

	Weapon->PullTrigger();
}

// Returns health percentage for health bar.
float APIXPlayer::GetHealthPercent() const
{
	return Health / DefaultHealth;
}

// Called when player takes damage.
float APIXPlayer::TakeDamage(float DamageAmount, struct FDamageEvent const &DamageEvent, class AController *EventInstigator, AActor *DamageCauser) 
{
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	DamageToApply = FMath::Min(Health, DamageToApply);
	Health -= DamageToApply;

	if(IsDead()){
		// AGEL_PIXGameModeBase* GameMode = GetWorld()->GetAuthGameMode<AGEL_PIXGameModeBase>();
		
		// if(GameMode != nullptr){
		// 	GameMode->PawnKilled(this);
		// }

		bDead = true;

		UE_LOG(LogTemp, Warning, TEXT("PLAYER DIED!"));

		DetachFromControllerPendingDestroy();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	return DamageToApply;
}

bool APIXPlayer::IsAiming() const
{
	return bAiming;
}

bool APIXPlayer::IsDead() const
{
	return Health <= 0;
}



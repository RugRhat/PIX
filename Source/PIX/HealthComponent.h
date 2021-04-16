// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

// OnHealthChanged event called when health changes for any reason.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FOnHealthChangedSignature, UHealthComponent*, OwningHealthComp, float, Health, float, HealthDelta, const class UDamageType*, DamageType, class AController*, InstigatedBy, AActor*, DamageCauser);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PIX_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

	// Event called when health changes for any reason.
	UPROPERTY(BlueprintAssignable)
	FOnHealthChangedSignature OnHealthChanged;

	/// TODO: Remove UPROPERTY for final build since using game instance team as official team.
	// Indicates which team player is on. Can be changed from editor.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	uint8 TeamNum;

	// Determines character's max health. Can be changed from editor.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float DefaultHealth;

	// Handles character healing.		// UFUNCTION for if called as a reward for killing a player.
	UFUNCTION(BlueprintCallable, Category = "HealthComponent")
	void Heal(float HealAmount);

	// Assigns player's team to TeamNum.
	void SetPlayerTeam();
	
	// Returns character's health.
	float GetHealth() const;

protected:
	// Players actual health that changes during gameplay.
	UPROPERTY(ReplicatedUsing = OnRep_Health, BlueprintReadOnly, Category = "HealthComponent")
	float Health;

	// Handles what happens when character health is replicated.
	UFUNCTION()
	void OnRep_Health(float OldHealth);

	// Called when the character sustains any damage.
	UFUNCTION()
	void HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
	
	// Called when the game starts or when spawned.
	virtual void BeginPlay() override;

	// Handles triggering actor killed event on active gamemode.
	void HandleGMDeath(APawn* Victim, class AController* KillerController);

	// Determines if given characters are on the same team (friendly).
	static bool IsFriendly(AActor* ActorA, AActor* ActorB);
		
	bool bIsDead;
};

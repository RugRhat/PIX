// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_UseWeapon.generated.h"

/**
 * 
 */
UCLASS()
class PIX_API UBTTask_UseWeapon : public UBTTaskNode
{
	GENERATED_BODY()

public:
	// Handles AI shooting.
	UBTTask_UseWeapon();

protected:
	// Called when behavior tree is ready to shoot at player.
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) override;		
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_UseWeapon.h"
#include "AIController.h"
#include "BaseCharacter.h"

// Handles AI shooting.
UBTTask_UseWeapon::UBTTask_UseWeapon() 
{
    NodeName = TEXT("UseWeapon");
}

EBTNodeResult::Type UBTTask_UseWeapon::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) 
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    if(OwnerComp.GetAIOwner() == nullptr){ return EBTNodeResult::Failed; }
    
    ABaseCharacter* Self = Cast<ABaseCharacter>(OwnerComp.GetAIOwner()->GetPawn());
    if(Self == nullptr){ return EBTNodeResult::Failed; }

    if(Self->GetAmmoCount() == 0)
    {
        Self->Reload();
    }
    else
    {
        Self->UseWeapon();
    }

    return EBTNodeResult::Succeeded;
}

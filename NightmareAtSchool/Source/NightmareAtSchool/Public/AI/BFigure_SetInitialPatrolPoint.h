// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BFigure_SetInitialPatrolPoint.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class NIGHTMAREATSCHOOL_API UBFigure_SetInitialPatrolPoint : public UBTTaskNode
{
	GENERATED_BODY()

public:
    UBFigure_SetInitialPatrolPoint();

    virtual EBTNodeResult::Type ExecuteTask(
        UBehaviorTreeComponent& OwnerComp,
        uint8* NodeMemory
    ) override;
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "SelectNextPatrolPoint.generated.h"

/**
 * 
 */
UCLASS()
class NIGHTMAREATSCHOOL_API USelectNextPatrolPoint : public UBTTaskNode
{
	GENERATED_BODY()
	

public:
    //USelectNextPatrolPoint();

    virtual EBTNodeResult::Type ExecuteTask(
        UBehaviorTreeComponent& OwnerComp,
        uint8* NodeMemory
    ) override;
};

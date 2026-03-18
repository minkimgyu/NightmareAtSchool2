// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BFigure_SelectNextPatrolPoint.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class NIGHTMAREATSCHOOL_API UBFigure_SelectNextPatrolPoint : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBFigure_SelectNextPatrolPoint();

	virtual EBTNodeResult::Type ExecuteTask(
		UBehaviorTreeComponent& OwnerComp,
		uint8* NodeMemory
	) override;
	
};

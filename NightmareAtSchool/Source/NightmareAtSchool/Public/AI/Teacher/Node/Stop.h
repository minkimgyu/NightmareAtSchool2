// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Stop.generated.h"

/**
 * 
 */
UCLASS()
class NIGHTMAREATSCHOOL_API UStop : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UStop();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};

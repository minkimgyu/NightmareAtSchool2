// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "IsTargetADoor.generated.h"

/**
 * 
 */
UCLASS()
class NIGHTMAREATSCHOOL_API UIsTargetADoor : public UBTDecorator
{
	GENERATED_BODY()
public:
	UIsTargetADoor();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};

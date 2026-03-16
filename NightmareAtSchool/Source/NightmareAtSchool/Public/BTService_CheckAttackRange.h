// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_CheckAttackRange.generated.h"

//class UDistanceToTargetComponent;

/**
 * 
 */
UCLASS()
class NIGHTMAREATSCHOOL_API UBTService_CheckAttackRange : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTService_CheckAttackRange();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(BlueprintReadOnly)
	float DistanceValue;

	//UPROPERTY()
	//UDistanceToTargetComponent* DistComp;
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "TurnTo.generated.h"

/**
 * 
 */
UCLASS()
class NIGHTMAREATSCHOOL_API UTurnTo : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UTurnTo();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	// 회전 속도 (높을수록 빠르게 돌아감)
	UPROPERTY(EditAnywhere, Category = "AI")
	float TurnSpeed = 10.0f;

	// 어느 정도 각도까지 정렬되면 성공으로 칠 것인가?
	UPROPERTY(EditAnywhere, Category = "AI")
	float Precision = 2.0f;

};

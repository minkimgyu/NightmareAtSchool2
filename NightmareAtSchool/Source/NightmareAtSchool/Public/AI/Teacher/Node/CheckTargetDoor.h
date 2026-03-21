// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CheckTargetDoor.generated.h"

/**
 * 
 */
UCLASS()
class NIGHTMAREATSCHOOL_API UCheckTargetDoor : public UBTService
{
	GENERATED_BODY()

public:
	UCheckTargetDoor();

protected:
	// 서비스가 실행될 때 주기적으로 호출되는 함수입니다.
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	// 블랙보드의 IsTargetADoor(Bool) 변수와 연결할 키
	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector IsTargetADoorKey;
};

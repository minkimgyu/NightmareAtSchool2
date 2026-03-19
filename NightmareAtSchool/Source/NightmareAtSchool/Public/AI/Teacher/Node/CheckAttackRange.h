// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CheckAttackRange.generated.h"

/**
 * 
 */
UCLASS()
class NIGHTMAREATSCHOOL_API UCheckAttackRange : public UBTService
{
	GENERATED_BODY()
	
public:
	UCheckAttackRange();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	// 에디터에서 설정할 공격 사거리
	UPROPERTY(EditAnywhere, Category = "AI")
	float AttackRange = 150.0f;

	// 블랙보드의 CanAttack(Bool) 변수와 연결할 키
	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector CanAttackKey;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "AttackPlayer.generated.h"

/**
 * 
 */
UCLASS()
class NIGHTMAREATSCHOOL_API UAttackPlayer : public UBTTaskNode
{
	GENERATED_BODY()

public:
    UAttackPlayer();

protected:
    // Task가 시작될 때 호출
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

    // Task 진행 중 매 프레임 호출 (애니메이션 대기용)
    virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

    // 에디터에서 할당할 공격 몽타주
    UPROPERTY(EditAnywhere, Category = "Attack")
    TObjectPtr<UAnimMontage> AttackMontage;

private:
    // 남은 애니메이션 시간을 추적
    float RemainingAnimationTime = 0.0f;
};

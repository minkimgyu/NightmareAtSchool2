// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Teacher/Node/ClearInvestigating.h"
#include "AI/Patrol/PatrolAIController.h" // 컨트롤러 헤더 추가 필수
#include "BehaviorTree/BlackboardComponent.h"

// BTTask_ClearValue.cpp
EBTNodeResult::Type UClearInvestigating::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // 1. AI 컨트롤러 가져오기
    APatrolAIController* AIC = Cast<APatrolAIController>(OwnerComp.GetAIOwner());
    UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
    if (AIC && BB)
    {
        float TargetSpeed = AIC->GetPatrolSpeed(); // 또는 AIC->PatrolSpeed;

        // 조사 속도로 블랙보드 값 변경
        BB->SetValueAsFloat(TEXT("MovementSpeed"), TargetSpeed);

        BB->ClearValue(TEXT("IsInvestigating")); // Bool의 경우 false가 됨
        return EBTNodeResult::Succeeded;
    }
    return EBTNodeResult::Failed;
}
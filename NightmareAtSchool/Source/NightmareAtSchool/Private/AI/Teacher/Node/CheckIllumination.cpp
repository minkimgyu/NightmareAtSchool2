// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Teacher/Node/CheckIllumination.h"
#include "AIController.h"

#include "AI/Patrol/PatrolAICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"


UCheckIllumination::UCheckIllumination()
{
    NodeName = "Check Illumination";
    // 서비스 실행 간격 설정 (매 틱이 아니라 일정 간격으로 최적화 가능)
    Interval = 0.1f;
    RandomDeviation = 0.05f;

}

void UCheckIllumination::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
    UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
    APatrolAICharacter* PatrolAI = Cast<APatrolAICharacter>(ControllingPawn);

    if (PatrolAI)
    {
        BB->SetValueAsBool(IsIlluminatedKey.SelectedKeyName, PatrolAI->IsIlluminated());
    }
}
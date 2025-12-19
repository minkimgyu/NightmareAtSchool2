// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/PatrolPoint.h"
#include "AI/PatrolAIController.h"
#include "AI/PatrolManager.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "AI/Node/SelectNextPatrolPoint.h"

EBTNodeResult::Type USelectNextPatrolPoint::ExecuteTask(
    UBehaviorTreeComponent& OwnerComp,
    uint8* NodeMemory)
{
    APatrolAIController* AICon = Cast<APatrolAIController>(OwnerComp.GetAIOwner());
    if (!AICon) return EBTNodeResult::Failed;

    APatrolManager* Manager = AICon->GetPatrolManager();
    UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();

    APatrolPoint* Current =
        Cast<APatrolPoint>(BB->GetValueAsObject(TEXT("CurrentPatrolPoint")));

    if (!Current) return EBTNodeResult::Failed;

    APatrolPoint* Next = Manager->GetNextPatrolPoint(Current);
    if (!Next) return EBTNodeResult::Failed;

    BB->SetValueAsObject(TEXT("CurrentPatrolPoint"), Next);
    BB->SetValueAsVector(TEXT("MoveTargetLocation"), Next->GetActorLocation());

    return EBTNodeResult::Succeeded;
}
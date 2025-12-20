// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Patrol/Node/SelectNextPatrolPoint.h"

#include "AI/Patrol/PatrolPoint.h"
#include "AI/Patrol/PatrolAIController.h"
#include "AI/Patrol/PatrolManager.h"

#include "BehaviorTree/BlackboardComponent.h"

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
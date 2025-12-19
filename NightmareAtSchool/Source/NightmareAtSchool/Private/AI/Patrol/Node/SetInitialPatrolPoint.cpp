// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Patrol/PatrolPoint.h"
#include "AI/Patrol/PatrolManager.h"
#include "AI/Patrol/PatrolAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "AI/Patrol/Node/SetInitialPatrolPoint.h"

USetInitialPatrolPoint::USetInitialPatrolPoint()
{
    NodeName = TEXT("Set Initial Patrol Point");
}

EBTNodeResult::Type USetInitialPatrolPoint::ExecuteTask(
    UBehaviorTreeComponent& OwnerComp,
    uint8* NodeMemory)
{
    auto* AICon = Cast<APatrolAIController>(OwnerComp.GetAIOwner());
    if (!AICon) return EBTNodeResult::Failed;

    auto* Manager = AICon->GetPatrolManager();
    if (!Manager) return EBTNodeResult::Failed;

    APatrolPoint* Start = Manager->GetRandomStartPoint();
    if (!Start) return EBTNodeResult::Failed;

    auto* BB = OwnerComp.GetBlackboardComponent();
    BB->SetValueAsObject(TEXT("CurrentPatrolPoint"), Start);
    BB->SetValueAsVector(TEXT("MoveTargetLocation"), Start->GetActorLocation());
    BB->SetValueAsBool(TEXT("IsPatrolInitialized"), true);

    return EBTNodeResult::Succeeded;
}
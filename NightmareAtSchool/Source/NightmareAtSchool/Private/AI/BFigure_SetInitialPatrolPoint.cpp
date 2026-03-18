// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BFigure_SetInitialPatrolPoint.h"

#include "AI/Patrol/PatrolPoint.h"
#include "AI/Patrol/PatrolManager.h"
#include "AGhost1AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBFigure_SetInitialPatrolPoint::UBFigure_SetInitialPatrolPoint()
{
    NodeName = TEXT("BFigure_SetInitialPatrolPoint");
}

EBTNodeResult::Type UBFigure_SetInitialPatrolPoint::ExecuteTask(
    UBehaviorTreeComponent& OwnerComp,
    uint8* NodeMemory)
{
    auto* AICon = Cast<AAGhost1AIController>(OwnerComp.GetAIOwner());
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
// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BFigure_SelectNextPatrolPoint.h"

#include "AI/Patrol/PatrolPoint.h"
#include "AGhost1AIController.h"
#include "AI/Patrol/PatrolManager.h"

#include "BehaviorTree/BlackboardComponent.h"

UBFigure_SelectNextPatrolPoint::UBFigure_SelectNextPatrolPoint()
{
    NodeName = TEXT("BFigure_SelectNextPatrolPoint");
}

EBTNodeResult::Type UBFigure_SelectNextPatrolPoint::ExecuteTask(
    UBehaviorTreeComponent& OwnerComp,
    uint8* NodeMemory)
{
    AAGhost1AIController* AICon = Cast<AAGhost1AIController>(OwnerComp.GetAIOwner());
    if (!AICon) return EBTNodeResult::Failed;

    ABasePatrolManager* Manager = AICon->GetPatrolManager();
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
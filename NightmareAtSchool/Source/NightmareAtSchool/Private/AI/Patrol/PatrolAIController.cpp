// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Patrol/PatrolManager.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "AI/Patrol/PatrolAIController.h"

APatrolAIController::APatrolAIController()
{
}

void APatrolAIController::BeginPlay()
{
    Super::BeginPlay();

    if (!PatrolBehaviorTree)
    {
        UE_LOG(LogTemp, Error, TEXT("PatrolBehaviorTree is NULL"));
        return;
    }

    const bool bBTStarted = RunBehaviorTree(PatrolBehaviorTree);

    UE_LOG(LogTemp, Error, TEXT("RunBehaviorTree result = %s"),
        bBTStarted ? TEXT("SUCCESS") : TEXT("FAIL"));
}

void APatrolAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    UE_LOG(LogTemp, Error, TEXT("Controller class: %s"), *GetClass()->GetName());

    // 월드에 배치된 PatrolManager 찾기
    TArray<AActor*> Found;
    UGameplayStatics::GetAllActorsOfClass(
        GetWorld(),
        APatrolManager::StaticClass(),
        Found
    );

    if (Found.Num() > 0)
    {
        PatrolManager = Cast<APatrolManager>(Found[0]);
    }
}
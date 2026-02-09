// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_CheckAttackRange.h"
//#include "DistanceToTargetComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_CheckAttackRange::UBTService_CheckAttackRange()
{
    bNotifyTick = true;
    Interval = 0.2f;   // 0.2초마다 계산하기
}

void UBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    UE_LOG(LogTemp, Warning, TEXT("TargetDistance Change"));
    AAIController* AICon = OwnerComp.GetAIOwner();
    if (!AICon) return;

    APawn* Pawn = AICon->GetPawn();
    if (!Pawn) return;

    ////거리를 구하는 컴포넌트를 받아옴
    //DistComp = Pawn->FindComponentByClass<UDistanceToTargetComponent>();
    //if (!DistComp) return;

    //DistanceValue = DistComp->GetDistanceToTarget(); // 거리 저장

    //블랙보드에 저장된 타겟의 위치정보를 받아옴
    auto* BB = OwnerComp.GetBlackboardComponent();
    if (!BB) return;

    AActor* Target = Cast<AActor>(BB->GetValueAsObject("Target"));
    if (!Target) return;

    DistanceValue = FVector::Dist(Pawn->GetActorLocation(),Target->GetActorLocation());

    BB->SetValueAsFloat("TargetDistance", DistanceValue);
}
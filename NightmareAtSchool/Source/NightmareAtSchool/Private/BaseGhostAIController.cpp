// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGhostAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MonsterViewAngleComponent.h"
#include "DistanceToTargetComponent.h"

void ABaseGhostAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	//트리가 비어있다면
	if (!BehaviorTree)
	{
		UE_LOG(LogTemp, Warning, TEXT("BehaviorTree is empty"));
		return;
	}

	if (UseBlackboard(BehaviorTree->BlackboardAsset, BlackboardComp)) // 여기서 블랙보드 초기화하고 AI컨트롤러쪽 연결
	{
		RunBehaviorTree(BehaviorTree); // 블랙보드를 준비한다음에 행동트리 실행
	}

	CollectAndStartEvent();
}

//기본으로 제공되는 함수 그대로 쓸거면 이거 사용
void ABaseGhostAIController::CollectAndStartEvent()
{
	MonsterViewAngleEvent();
	DistanceToTargetEvent();
}

//1. 시야각 이벤트
void ABaseGhostAIController::MonsterViewAngleEvent()
{
	MonsterViewAngleComp = GetPawn()->FindComponentByClass<UMonsterViewAngleComponent>();

	if (MonsterViewAngleComp)
	{
		MonsterViewAngleComp->OnTarget.AddDynamic(this, &ABaseGhostAIController::SetTarget);
		//MonsterViewAngleComp->Target
	}
}

void ABaseGhostAIController::DistanceToTargetEvent()
{
	DistanceToTargetComp = GetPawn()->FindComponentByClass<class UDistanceToTargetComponent>();

	if (DistanceToTargetComp)
	{
		//DistanceToTargetComp->OnTarget.AddDynamic(this, &ABaseGhostAIController::SetTarget);
	}
}


//고스트의 공통 기능
//1. 시야각 ---------------------------------------------------------
void ABaseGhostAIController::SetTarget(AActor* NewTarget)
{
	Target = NewTarget;  // nullptr도 가능
	SaveTargetBlackboard();
}

void ABaseGhostAIController::SaveTargetBlackboard()
{
	if (!BlackboardComp)
	{
		return;
	}
	BlackboardComp = GetBlackboardComponent();

	if (Target)
	{
		if (BlackboardComp)
		{
			UE_LOG(LogTemp, Warning, TEXT("SUCCESS BLACKBOARD"));
			BlackboardComp->SetValueAsObject(TEXT("Target"), Target);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Clear"));
		BlackboardComp->ClearValue(TEXT("Target"));
	}
}

//2. 고스트 거리구하기--------------------------------------------


//
//void ABaseGhostAIController::GetAIController()
//{
//	UE_LOG(LogTemp, Warning, TEXT("GetAIController"));
//	OwnerPawn = Cast<APawn>(GetOwner());
//	if (!OwnerPawn)
//	{
//		return;
//	}
//
//	AIController = Cast<AAIController>(OwnerPawn->GetController());
//	if (!AIController)
//	{
//		UE_LOG(LogTemp, Warning, TEXT("DISCOVER AICONTROLLER"));
//		return;
//	}
//}
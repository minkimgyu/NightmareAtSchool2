// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Teacher/Node/CheckAttackRange.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Pawn.h"

UCheckAttackRange::UCheckAttackRange()
{
	NodeName = TEXT("Check Attack Range");
	Interval = 0.1f; // 0.1초마다 체크 (성능 최적화)
}

void UCheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* AIC = OwnerComp.GetAIOwner();
	APawn* AIPawn = AIC->GetPawn();
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();

	if (!AIPawn || !BB) return;

	// 블랙보드에서 타겟 액터(플레이어) 가져오기
	AActor* Target = Cast<AActor>(BB->GetValueAsObject(TEXT("TargetActor")));
	if (!Target)
	{
		BB->SetValueAsBool(CanAttackKey.SelectedKeyName, false);
		return;
	}

	// 거리 계산
	float Distance = FVector::Dist(AIPawn->GetActorLocation(), Target->GetActorLocation());

	// 2. 공격 사거리를 시각적으로 확인하기 위해 디버그 구체를 그립니다. (선택 사항)
	DrawDebugSphere(AIPawn->GetWorld(), AIPawn->GetActorLocation(), AttackRange, 12, FColor::Red, false, 0.1f);

	// 사거리 안에 들어왔는지 유무를 블랙보드에 저장
	bool bCanAttack = (Distance <= AttackRange);
	BB->SetValueAsBool(CanAttackKey.SelectedKeyName, bCanAttack);
}
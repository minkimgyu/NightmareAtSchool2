// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Teacher/Node/TurnTo.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

UTurnTo::UTurnTo()
{
	NodeName = TEXT("Turn To Player");
	bNotifyTick = true; // 실시간 회전을 위해 틱 활성화
}

EBTNodeResult::Type UTurnTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return EBTNodeResult::InProgress; // 성공할 때까지 계속 Tick 실행
}

void UTurnTo::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* AIC = OwnerComp.GetAIOwner();
	APawn* AIPawn = AIC->GetPawn();
	AActor* Target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("TargetActor")));

	if (!AIPawn || !Target)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	// 1. 목표 방향 계산
	FVector Dir = Target->GetActorLocation() - AIPawn->GetActorLocation();
	Dir.Z = 0.0f; // 수평 회전만 고려
	FRotator TargetRot = Dir.Rotation();

	// 2. 부드러운 회전 적용 (RInterp)
	FRotator NewRot = FMath::RInterpTo(AIPawn->GetActorRotation(), TargetRot, DeltaSeconds, TurnSpeed);
	AIPawn->SetActorRotation(NewRot);

	// 3. 정밀도 범위 안에 들어왔는지 확인
	float AngleDiff = FMath::Abs(FMath::FindDeltaAngleDegrees(AIPawn->GetActorRotation().Yaw, TargetRot.Yaw));
	if (AngleDiff <= Precision)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
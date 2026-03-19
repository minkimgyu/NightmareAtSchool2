// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Teacher/Node/AttackPlayer.h"

#include "AIController.h"
#include "GameFramework/Character.h"
#include "Components/InteractionComponent.h"

UAttackPlayer::UAttackPlayer()
{
    NodeName = TEXT("Attack Player");
    bNotifyTick = true; // TickTask 활성화
}

EBTNodeResult::Type UAttackPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIC = OwnerComp.GetAIOwner();
    ACharacter* AIPawn = Cast<ACharacter>(AIC->GetPawn());

    if (!AIPawn || !AttackMontage) return EBTNodeResult::Failed;

    // 1. 공격 애니메이션 재생 (몽타주 재생 시간 반환)
    RemainingAnimationTime = AIPawn->PlayAnimMontage(AttackMontage);

    // 로그 추가
    UE_LOG(LogTemp, Warning, TEXT("Montage Play Length: %f"), RemainingAnimationTime);

    if (RemainingAnimationTime <= 0.0f) return EBTNodeResult::Succeeded;


    // 2. 실질적인 데미지 판정 (표준 방식)
    //AActor* Target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("TargetActor")));
    //if (Target)
    //{
    //    // 간단한 거리/각도 체크 후 데미지 입히기
    //    float Distance = FVector::Dist(AIPawn->GetActorLocation(), Target->GetActorLocation());

    //    // 사거리(예: 150) 안에 있다면 데미지 전달
    //    if (Distance <= 150.0f)
    //    {
    //        // ApplyDamage(피해액터, 피해량, 가해컨트롤러, 가해액터, 데미지타입클래스)
    //        UGameplayStatics::ApplyDamage(
    //            Target,           // 플레이어
    //            10.0f,            // 데미지 수치
    //            AIC,              // AI 컨트롤러
    //            AIPawn,           // AI 자신
    //            UDamageType::StaticClass()
    //        );

    //        UE_LOG(LogTemp, Warning, TEXT("AI Attacked Player! Damage: 10.0"));
    //    }
    //}


    return EBTNodeResult::InProgress; // 애니메이션이 끝날 때까지 대기
}

void UAttackPlayer::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    RemainingAnimationTime -= DeltaSeconds;

    if (RemainingAnimationTime <= 0.0f)
    {
        // 애니메이션이 끝나면 태스크 성공 종료
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    }
}
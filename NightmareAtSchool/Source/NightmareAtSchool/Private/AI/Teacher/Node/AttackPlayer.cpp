// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Teacher/Node/AttackPlayer.h"

#include "AIController.h"
#include "GameFramework/Character.h"

#include "Kismet/KismetSystemLibrary.h" // BoxOverlapActors 사용을 위해 필요
#include "Kismet/GameplayStatics.h"

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

    // 1. 공격 애니메이션 재생
    RemainingAnimationTime = AIPawn->PlayAnimMontage(AttackMontage);

    if (RemainingAnimationTime <= 0.0f) return EBTNodeResult::Succeeded;

    // 2. 상자 범위 탐색 및 데미지 로직 실행
    //PerformBoxOverlapAttack(AIPawn, AIC);

    return EBTNodeResult::InProgress;
}

void UAttackPlayer::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    RemainingAnimationTime -= DeltaSeconds;

    if (RemainingAnimationTime <= 0.0f)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    }
}

// 상자 범위 탐색 함수 구현
void UAttackPlayer::PerformBoxOverlapAttack(ACharacter* Attacker, AAIController* Controller)
{
    if (!Attacker) return;

    // [설정] SD 도트 캐릭터 특성에 맞춘 오프셋과 크기
    // 정면으로 70cm 지점, 가로100x세로100x높이100 박스 (절반값인 50 사용)
    FVector Center = Attacker->GetActorLocation() + Attacker->GetActorForwardVector() * AttackOffset;
    FRotator Rotation = Attacker->GetActorRotation();

    // 탐색할 객체 타입 설정 (Pawn)
    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

    TArray<AActor*> IgnoreActors;
    IgnoreActors.Add(Attacker); // 자기 자신 제외

    TArray<AActor*> OutActors;

    // 범위 내 액터 검출
    bool bHit = UKismetSystemLibrary::BoxOverlapActors(
        Attacker->GetWorld(),
        Center,
        BoxHalfSize,
        ObjectTypes,
        nullptr, // 모든 액터 클래스 허용
        IgnoreActors,
        OutActors
    );

    // ⭐ 디버그 변수가 true일 때만 박스를 그립니다.
    if (bShowDebugBox)
    {
        // 디버그용 박스 표시 (에디터에서 확인용, 1초간 유지)
        UKismetSystemLibrary::DrawDebugBox(Attacker->GetWorld(), Center, BoxHalfSize, DebugBoxColor, Rotation, 1.0f, 2.0f);
    }

    if (bHit)
    {
        for (AActor* HitActor : OutActors)
        {
            UGameplayStatics::ApplyDamage(
                HitActor,
                AttackDamage, // 데미지 수치
                Controller,
                Attacker,
                UDamageType::StaticClass()
            );

            UE_LOG(LogTemp, Warning, TEXT("Hit Target: %s"), *HitActor->GetName());
        }
    }
}
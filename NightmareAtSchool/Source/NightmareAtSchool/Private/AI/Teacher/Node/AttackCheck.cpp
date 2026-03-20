// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Teacher/Node/AttackCheck.h"

#include "AI/Teacher/Node/AttackPlayer.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BTTaskNode.h"

void UAttackCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    Super::Notify(MeshComp, Animation);

    if (!MeshComp) return;

    // 1. 캐릭터와 AI 컨트롤러 가져오기
    ACharacter* OwnerCharacter = Cast<ACharacter>(MeshComp->GetOwner());
    if (!OwnerCharacter) return;

    AAIController* AIC = Cast<AAIController>(OwnerCharacter->GetController());
    if (!AIC) return;

    // 2. 현재 실행 중인 비헤이비어 트리 태스크 찾기
    // 이 방식은 현재 캐릭터가 수행 중인 태스크 중 UAttackPlayer 타입을 찾아 함수를 실행합니다.
    UBehaviorTreeComponent* BTComp = Cast<UBehaviorTreeComponent>(AIC->GetBrainComponent());
    if (BTComp)
    {
        // 에러 해결 : const_cast를 사용하여 const 제약 해제
        // GetActiveNode()가 반환하는 const UBTPartial로 부터 안전하게 캐스팅
        UAttackPlayer * AttackTask = const_cast<UAttackPlayer*>(Cast<UAttackPlayer>(BTComp->GetActiveNode()));

        if (AttackTask)
        {
            // 드디어 상자 탐색 함수 호출!
            AttackTask->PerformBoxOverlapAttack(OwnerCharacter, AIC);
        }
    }
}
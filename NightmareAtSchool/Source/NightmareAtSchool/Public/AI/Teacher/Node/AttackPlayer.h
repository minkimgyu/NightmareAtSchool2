// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "AttackPlayer.generated.h"


/**
 * 
 */
UCLASS()
class NIGHTMAREATSCHOOL_API UAttackPlayer : public UBTTaskNode
{
	GENERATED_BODY()

public:
    UAttackPlayer();

    void PerformBoxOverlapAttack(ACharacter* Attacker, AAIController* Controller);

protected:
    // Task가 시작될 때 호출
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

    // Task 진행 중 매 프레임 호출 (애니메이션 대기용)
    virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;


    // 공격 데미지 수치 (BT 에디터에서 수정 가능)
    UPROPERTY(EditAnywhere, Category = "Combat")
    float AttackDamage = 40.0f;

    /** [추가] 공격 판정 박스의 크기 (Half Size) */
    UPROPERTY(EditAnywhere, Category = "Attack|Collision")
    FVector BoxHalfSize = FVector(100.0f, 100.0f, 50.0f);

    /** [추가] 캐릭터 중심으로부터 판정 박스까지의 거리(앞방향) */
    UPROPERTY(EditAnywhere, Category = "Attack|Collision")
    float AttackOffset = 70.0f;

    // 에디터에서 할당할 공격 몽타주
    UPROPERTY(EditAnywhere, Category = "Attack")
    TObjectPtr<UAnimMontage> AttackMontage;


    /** ⭐ 공격 판정 박스 디버그 표시 여부 */
    UPROPERTY(EditAnywhere, Category = "Attack|Collision|Debug")
    bool bShowDebugBox = true;

    /** ⭐ 디버그 박스 색상 */
    UPROPERTY(EditAnywhere, Category = "Attack|Collision|Debug")
    FLinearColor DebugBoxColor = FLinearColor::Blue;

private:
    // 남은 애니메이션 시간을 추적
    float RemainingAnimationTime = 0.0f;
};

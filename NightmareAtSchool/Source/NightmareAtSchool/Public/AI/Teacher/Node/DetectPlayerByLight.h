// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "DetectPlayerByLight.generated.h"

/**
 * 
 */
UCLASS()
class NIGHTMAREATSCHOOL_API UDetectPlayerByLight : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
    UDetectPlayerByLight();

protected:
    // 태스크 실행 시 호출되는 함수
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

    // 에디터에서 플레이어를 저장할 블랙보드 키 (Object 타입)
    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector TargetActorKey;
};

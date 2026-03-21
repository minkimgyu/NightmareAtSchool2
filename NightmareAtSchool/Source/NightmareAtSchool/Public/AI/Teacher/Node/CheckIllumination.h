// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CheckIllumination.generated.h"

/**
 * 
 */
UCLASS()
class NIGHTMAREATSCHOOL_API UCheckIllumination : public UBTService
{
	GENERATED_BODY()

public:
    UCheckIllumination();

protected:
    // 서비스가 틱마다 실행될 함수
    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

    // 에디터에서 선택할 블랙보드 키 (Bool 타입)
    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector IsIlluminatedKey;
};

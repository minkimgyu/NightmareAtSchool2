// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "OpenDoor.generated.h"

/**
 * 
 */
UCLASS()
class NIGHTMAREATSCHOOL_API UOpenDoor : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UOpenDoor();

protected:
	/** 태스크가 실행될 때 호출되는 메인 함수 */
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BaseGhostAIController.generated.h"

class UMonsterViewAngleComponent;
class UDistanceToTargetComponent;

/**
 * 
 */
UCLASS()
class NIGHTMAREATSCHOOL_API ABaseGhostAIController : public AAIController
{
	GENERATED_BODY()

protected:
	virtual void OnPossess(APawn* InPawn) override;

	//void GetAIController();
	void SaveTargetBlackboard();

	void CollectAndStartEvent(); // 이벤트들을 모은 뒤 그 이벤트들을 실행
	//실행 목록
	void MonsterViewAngleEvent();
	void DistanceToTargetEvent();
	//----------------------------

	UPROPERTY()
	UMonsterViewAngleComponent* MonsterViewAngleComp;

	UPROPERTY()
	UDistanceToTargetComponent* DistanceToTargetComp;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
	UBehaviorTree* BehaviorTree;

	//이벤트로 받아올 타겟 설정
	UFUNCTION(BlueprintCallable)
	void SetTarget(AActor* NewTarget);
	
	APawn* OwnerPawn;
	UBlackboardComponent* BlackboardComp;

	AActor* Target;
	
};
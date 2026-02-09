// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BaseGhostAIController.h"
#include "AGhost1AIController.generated.h"

class UBehaviorTree;
class GhostStateBase;
enum class EGhostState;
/**
 * 
 */
UCLASS()
class NIGHTMAREATSCHOOL_API AAGhost1AIController : public ABaseGhostAIController
{
	GENERATED_BODY()

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void Tick(float DeltaTime) override;

	EGhostState CurrentState;

	GhostStateBase* GStateBase;
public:

//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
//	UBehaviorTree* BehaviorTree;
//
//	//이벤트로 받아올 타겟 설정
//	UFUNCTION(BlueprintCallable)
//	void SetTarget(AActor* NewTarget);
//	
//	APawn* OwnerPawn;
//	UBlackboardComponent* BlackboardComp;
//
//	AActor* Target;
//	
//private:
//	void GetAIController();
//	void SaveBlackboard();
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Teacher/Node/Stop.h"

#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"

UStop::UStop()
{
	NodeName = TEXT("Stop Movement");
}

EBTNodeResult::Type UStop::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIC = OwnerComp.GetAIOwner();
	if (ACharacter* AIPawn = Cast<ACharacter>(AIC->GetPawn()))
	{
		// 1. 경로 이동 중단
		AIC->StopMovement();

		// 2. 가속도 강제 초기화 (즉시 정지)
		if (UCharacterMovementComponent* MoveComp = AIPawn->GetCharacterMovement())
		{
			MoveComp->StopMovementImmediately();
		}

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "Ghost1.h"

UBTTask_Attack::UBTTask_Attack()
{
	NodeName = TEXT("Attack");
}

//노드 실행
EBTNodeResult::Type UBTTask_Attack::ExecuteTask(
	UBehaviorTreeComponent& OwnerComp, 
	uint8* NodeMemory
)
{

	AAIController* AICon = OwnerComp.GetAIOwner();
	if (!AICon)
	{
		return EBTNodeResult::Failed; // 실패 반환
	}

	UE_LOG(LogTemp, Warning, TEXT("Game Over"));

	AGhost1* Ghost = Cast<AGhost1>(AICon->GetPawn());

	if (!Ghost)
	{
		return EBTNodeResult::Failed; // 실패 반환
	}

	//실제 실행 내용물


	return EBTNodeResult::Succeeded; // 성공 반환
}


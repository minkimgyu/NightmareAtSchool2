// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Teacher/Node/OpenDoor.h"
#include "AIController.h"
#include "Components/InteractionComponent.h"
#include "GameFramework/Pawn.h"

UOpenDoor::UOpenDoor()
{
	NodeName = TEXT("Open Door");
}

EBTNodeResult::Type UOpenDoor::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// 1. AI 컨트롤러와 폰(Pawn) 가져오기
	AAIController* AIC = OwnerComp.GetAIOwner();
	if (!AIC) return EBTNodeResult::Failed;

	APawn* AIPawn = AIC->GetPawn();
	if (!AIPawn) return EBTNodeResult::Failed;

	// 2. 캐릭터 내부의 InteractionComponent 찾기
	UInteractionComponent* InteractionComp = AIPawn->FindComponentByClass<UInteractionComponent>();

	if (InteractionComp)
	{
		// 4. 상호작용 시작 (문 열기 명령 전달)
		InteractionComp->BeginInteract();
		InteractionComp->EndInteract();

		UE_LOG(LogTemp, Log, TEXT("BTTask_OpenDoor: Successfully triggered interaction with Door."));
		return EBTNodeResult::Succeeded;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("BTTask_OpenDoor: InteractionComponent not found on AI Pawn."));
	}

	return EBTNodeResult::Failed;
}
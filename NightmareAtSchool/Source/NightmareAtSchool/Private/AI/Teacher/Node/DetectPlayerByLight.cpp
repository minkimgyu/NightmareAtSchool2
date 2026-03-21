// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Teacher/Node/DetectPlayerByLight.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

UDetectPlayerByLight::UDetectPlayerByLight()
{
	NodeName = "Find Player";
}

EBTNodeResult::Type UDetectPlayerByLight::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB) return EBTNodeResult::Failed;

	// 1. PlayerController에서 플레이어 폰(Pawn)을 가져옴
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (PC && PC->GetPawn())
	{
		// 2. 블랙보드의 TargetActor 키에 플레이어를 등록
		BB->SetValueAsObject(TargetActorKey.SelectedKeyName, PC->GetPawn());
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
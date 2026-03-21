// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Teacher/Node/CheckTargetDoor.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/InteractionComponent.h"
#include "InteractableObject/DoorBase.h"
#include "GameFramework/Character.h"

UCheckTargetDoor::UCheckTargetDoor()
{
    NodeName = TEXT("Check Target Door");

    // 서비스의 실행 간격을 설정합니다. (0.1초 정도면 충분히 빠릅니다)
    Interval = 0.1f;
    RandomDeviation = 0.05f;
}

void UCheckTargetDoor::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* AIC = OwnerComp.GetAIOwner();
	if (!AIC) return;

	APawn* AIPawn = AIC->GetPawn();
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!AIPawn || !BB) return;

	bool bIsClosedDoorFound = false;

	// 1. InteractionComponent를 통해 현재 상호작용 가능한 대상 확인
	UInteractionComponent* InteractionComp = AIPawn->FindComponentByClass<UInteractionComponent>();
	if (InteractionComp)
	{
		AActor* CurrentTarget = InteractionComp->GetCurrentInteractable();

		// 2. 태그 및 문 클래스/상태 확인
		if (CurrentTarget && CurrentTarget->ActorHasTag(TEXT("OpenableDoor")))
		{
			if (ADoorBase* Door = Cast<ADoorBase>(CurrentTarget))
			{
				// 문이 닫혀있는지(IsOpen == false) 확인
				bIsClosedDoorFound = !Door->GetIsOpen();
			}
		}
	}

	// 3. 선택된 블랙보드 키에 결과값 저장
	BB->SetValueAsBool(IsTargetADoorKey.SelectedKeyName, bIsClosedDoorFound);
}
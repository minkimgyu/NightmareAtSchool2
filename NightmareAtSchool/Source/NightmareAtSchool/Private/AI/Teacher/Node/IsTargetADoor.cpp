// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Teacher/Node/IsTargetADoor.h"

#include "AIController.h"
#include "Components/InteractionComponent.h"
#include "GameFramework/Character.h"

// 문 클래스에 접근하기 위해 헤더 추가
#include "InteractableObject/DoorBase.h"

UIsTargetADoor::UIsTargetADoor()
{
	NodeName = TEXT("Is Target A Closed Door?");
}

bool UIsTargetADoor::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AAIController* AIC = OwnerComp.GetAIOwner();
	if (!AIC) return false;

	APawn* AIPawn = AIC->GetPawn();
	if (!AIPawn) return false;

	// 캐릭터에서 InteractionComponent를 찾습니다.
	UInteractionComponent* InteractionComp = AIPawn->FindComponentByClass<UInteractionComponent>();
	if (InteractionComp)
	{
		// InteractionComponent의 내부 데이터를 직접 확인하거나 헬퍼 함수를 호출합니다.
		// 제공된 코드의 InteractionData.CurrentInteractable를 활용합니다.
		AActor* CurrentTarget = InteractionComp->GetCurrentInteractable(); // Getter가 필요합니다.

		if (CurrentTarget && CurrentTarget->ActorHasTag(TEXT("OpenableDoor")))
		{
			// 2. ADoorBase로 캐스팅하여 문 상태 확인
			if (ADoorBase* Door = Cast<ADoorBase>(CurrentTarget))
			{
				// 문이 닫혀있을(bIsOpen == false) 때만 true를 반환하여 태스크 실행 허용
				return !Door->GetIsOpen();
			}
		}
	}

	return false;
}
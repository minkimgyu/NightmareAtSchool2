// Fill out your copyright notice in the Description page of Project Settings.


#include "Ghost11234SoundService.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"

UGhost11234SoundService::UGhost11234SoundService()
{
	NodeName = TEXT("Update Monster Sound");
	Interval = 0.2f; // 0.2초 체크
}

void UGhost11234SoundService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* Monster = OwnerComp.GetAIOwner()->GetPawn();
	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	if (!Monster || !BBComp) return;

	// Ghost1234의 오디오 컴포넌트 받아오고 
	UAudioComponent* AudioComp = Monster->FindComponentByClass<UAudioComponent>();
	if (!AudioComp) return;

	// 현재 state 체크
	bool bIsIlluminated = BBComp->GetValueAsBool(TEXT("IsIlluminated")); // 손전등에 맞았을때 경직상태
	bool bIsInvestigating = BBComp->GetValueAsBool(TEXT("IsInvestigating")); // 배회 이동
	bool bCanAttackKey = BBComp->GetValueAsBool(TEXT("CanAttackKey")); // 공격
	bool bIsChasing = BBComp->GetValueAsBool(TEXT("IsChasing")); // 추격

	USoundCue* TargetSound = nullptr;

	if (bIsChasing)
	{
		// 추격 중일 때 (가장 우선순위 높음)
		TargetSound = ChaseSound;
	}
	else
	{
		// 아무것도 아닐 때 (기본 대기 소리)
		TargetSound = BaseSound;
	}

	// 공격일 때는 일회성 소리를 나게하자
	if (bCanAttackKey)
	{
		if (AudioComp->IsPlaying()) AudioComp->Stop();
		return;
	}

	// 상태기반 사운드 교체시스템
	if (TargetSound && AudioComp->GetSound() != TargetSound)
	{
		AudioComp->Stop();
		AudioComp->SetSound(TargetSound);
		AudioComp->Play();
		UE_LOG(LogTemp, Warning, TEXT("Sound Playing!"));
	}
}
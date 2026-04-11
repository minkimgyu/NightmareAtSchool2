// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "Ghost11234SoundService.generated.h"

class USoundCue;

/**
 * 
 */
UCLASS()
class NIGHTMAREATSCHOOL_API UGhost11234SoundService : public UBTService
{
	GENERATED_BODY()
	
public:
	UGhost11234SoundService();
	
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundCue* BaseSound; // 기본 사운드

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundCue* ChaseSound; // 플레이어 추격할 때 사운드

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundCue* StunSound; // 기절 사운드

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundCue* InvestigateSound; // 이동 사운드

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector StateKey;
};

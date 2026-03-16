// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Quest/QuestObjectiveData.h"
#include "KillObjectiveData.generated.h"

/**
 * 
 */
UCLASS(EditInlineNew, BlueprintType)
class NIGHTMAREATSCHOOL_API UKillObjectiveData : public UQuestObjectiveData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Quest")
	FName MonsterID; // 처리해야할 몬스터의 고유ID

	virtual void OnEvent(FName TragetEvent, int32 Amount) override;
	
};

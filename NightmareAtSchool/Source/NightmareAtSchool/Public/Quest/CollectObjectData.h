// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Quest/QuestObjectiveData.h"
#include "CollectObjectData.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class NIGHTMAREATSCHOOL_API UCollectObjectData : public UQuestObjectiveData
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName ItemID; // 모아야하는 아이템 id

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 TargetCount = 1;
	
};

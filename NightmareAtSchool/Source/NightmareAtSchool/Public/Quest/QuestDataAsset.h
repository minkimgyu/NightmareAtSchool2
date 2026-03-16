// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "QuestObjectiveData.h"
#include "QuestDataAsset.generated.h"

/**
 * 
 */

UCLASS(BlueprintType)
class NIGHTMAREATSCHOOL_API UQuestDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName QuestID; // 퀘스트 고유 id

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText QuestName; // 퀘스트의 제목

	UPROPERTY(EditDefaultsOnly, Instanced, BlueprintReadOnly)
	TArray<UQuestObjectiveData*> Objectives;
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "QuestData.h"
#include "QuestSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class NIGHTMAREATSCHOOL_API UQuestSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	//플레이어가 진행 중인 퀘스트 목록
	UPROPERTY()
	TMap<FName, FActiveQuest> SavedActiveQuests;

	//플레이어가 완료한 퀘스트 목록
	UPROPERTY()
	TSet<FName> SavedCompletedQuestIDs;

	UPROPERTY()
	TSet<FName> SavedAvailableQuestIDS;
	
};

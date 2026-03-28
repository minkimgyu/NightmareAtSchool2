// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "QuestData.h"
#include "GameEventPostOfficeSubsystem.generated.h"

//퀘스트 구독 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FCallQuest, EQuestType, QuestType, FName, TargetID, int32, Amount); // 퀘스트타입, 타켓의 고유 ID, 타겟 갯수 (ex 아이템의 갯수, 킬 수등등)

/**
 * 
 */
UCLASS()
class NIGHTMAREATSCHOOL_API UGameEventPostOfficeSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Events")
	FCallQuest CallQuest;
};

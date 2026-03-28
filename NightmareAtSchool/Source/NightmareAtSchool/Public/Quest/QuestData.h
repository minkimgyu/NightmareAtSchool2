// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "QuestObjectiveData.h"
#include "Engine/DataTable.h"
#include "CoreMinimal.h"
#include "QuestData.generated.h"

/**
 * 
 */

//퀘스트의 종류
UENUM(BlueprintType)
enum class EQuestType : uint8 { Kill, Collect, ReachLocation};

//퀘스트 상세 정보
USTRUCT(BlueprintType)
struct FQuestObjective
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EQuestType QuestType = EQuestType::Kill; // 어떤 류의 퀘스트인가?

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName TargetID; // 해야할 것

    //도착정보 or 필요정도
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 RequiredCount = 100;
};

//데이터 테이블에서 행에 들어갈 정보들
USTRUCT(BlueprintType)
struct FQuestRow : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText QuestName; // 퀘스트의 이름

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FQuestObjective> Objectives; // 수행해야할 목표들

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText QuestExplain; // 퀘스트 설명

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText QuestExplainSummary; // 퀘스트 요약설명

    //------------------------------------------------
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText ItemName; // 해당 퀘스트 실행 전 발동 조건 아이템

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UTexture2D* ItemImage; // 해당 퀘스트 실행 전 발동 조건 아이템의 이미지

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText ThisQuestExplainToPlayer; // 해당 퀘스트 실행 전 설명

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 RewardGold = 0;
    
};

USTRUCT(BlueprintType)
struct FActiveQuest
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Quest")
    FName QuestID; // 퀘스트 ID

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Quest")
    TArray<int32> CurrentCounts; // 현재 얼마나 진행했는 지를 나타내는 변수들

    bool IsComplete(const FQuestRow* OriginalData) const // 완료했는지를 따지는 함수
    {
        if (!OriginalData)
            return false;

        for (int32 i = 0; i < OriginalData->Objectives.Num(); ++i)
        {
            if (CurrentCounts[i] < OriginalData->Objectives[i].RequiredCount)
                return false;
        }
        return true;
    }
};
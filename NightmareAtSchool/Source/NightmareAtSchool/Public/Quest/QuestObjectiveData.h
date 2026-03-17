// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "QuestObjectiveData.generated.h"

/**
 * 
 */

UCLASS(Abstract, BlueprintType, EditInlineNew, DefaultToInstanced)
class NIGHTMAREATSCHOOL_API UQuestObjectiveData : public UObject
{
	GENERATED_BODY()

public:

	virtual void OnEvent(FName EventID, int32 Amount);

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	//FName QuestID; // 퀘스트 고유 번호

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest")
	//FText QuestName; // 퀘스트 제목

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest")
	//FText QuestDescription; // 퀘스트 설명

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	int32 RequiredAmount; // 필요한 갯수 or 맵 id

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	int32 CurrentAmount; // 현재 상태

	// 퀘스트 달성 여부 확인
	UFUNCTION(BlueprintCallable)
	bool IsCompleted() const;

	//// 진행도를 올리는 함수 (자식에서 호출하거나 매니저가 호출)
	//virtual void UpdateProgress(int32 Amount);

};


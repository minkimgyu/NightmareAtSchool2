// Fill out your copyright notice in the Description page of Project Settings.


#include "Quest/QuestObjectiveData.h"
//
void UQuestObjectiveData::OnEvent(FName EventID, int32 Amount)
{
}

bool UQuestObjectiveData::IsCompleted() const
{
	return CurrentAmount == RequiredAmount; // >=는 특정 맵id 때문에 문제생길 수 있음
}
//
//void UQuestObjectiveData::UpdateProgress(int32 Amount)
//{
//	CurrentAmount = FMath::Clamp(CurrentAmount + Amount, 0, RequiredAmount); // 특정 값을 넘지 않게 하기 단, 맵 도착 퀘스트에서는 수정이 필요
//}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "QuestData.h"
#include "QueseManagerCompoent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnQuestUpdated);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NIGHTMAREATSCHOOL_API UQuestManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UQuestManagerComponent();

	//퀘스트 수락
	UFUNCTION(BlueprintCallable)
	void AcceptQuest(FName QuestID);

	//퀘스트 진행도
	void NotifyProgress(EQuestType Type, FName TargetID, int32 Amount);

	//퀘스트 UI 정보 갱신 함수
	//TArray<FText> GetActiveQuestDescriptions();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UPROPERTY(BlueprintAssignable, Category = "Quest")
	FOnQuestUpdated OnQuestUpdated;

	UPROPERTY(EditAnywhere, Category = "Quest")
	UDataTable* QuestDataTable; // 퀘스트 데이터 테이블 저장 변수

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	TMap<FName, FActiveQuest> ActiveQuests; // 수락한 퀘스트의 목록

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	TSet<FName> CompletedQuestIDs; // 수락한 퀘스트의 목록

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	TSet<FName> AvailableQuestIDs; // 진행가능한 퀘스트의 목록

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest") // bool값 수정하면서 처리할거니까
	bool isChangeActiveQuest = true;

private:
	FTimerHandle TimerHandle; // 자동 저장용 타이머

private:
	//진행 가능 퀘스트 초기화
	void InitializedAvailableQuest();

	//퀘스트를 저장하는 함수
	UFUNCTION(BlueprintCallable, Category = "Save")
	void SaveQuestData();

	//저장된 퀘스트를 불러오는 함수
	UFUNCTION(BlueprintCallable, Category = "Load")
	void LoadQuestData();

	//자동 저장 함수
	void AutoSave();

};

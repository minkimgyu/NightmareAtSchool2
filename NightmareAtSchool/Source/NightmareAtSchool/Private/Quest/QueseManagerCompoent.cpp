// Fill out your copyright notice in the Description page of Project Settings.

#include "Quest/QueseManagerCompoent.h"
#include "Kismet/GameplayStatics.h"
#include "Quest/QuestSaveGame.h"
#include "Quest/GameEventPostOfficeSubsystem.h"

// Sets default values for this component's properties
UQuestManagerComponent::UQuestManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	//LoadQuestData();
	// ...
}


// Called when the game starts
void UQuestManagerComponent::BeginPlay()
{
	Super::BeginPlay();
	//AutoSave();
	InitializedAvailableQuest();

	if (UGameInstance* GI = GetOwner()->GetGameInstance())
	{
		if (UGameEventPostOfficeSubsystem* EventSubsystem = GI->GetSubsystem<UGameEventPostOfficeSubsystem>())
		{
			//UE_LOG(LogTemp, Warning, TEXT("MTEMTEMTEMTEMTE"));
			EventSubsystem->CallQuest.AddDynamic(this, &UQuestManagerComponent::NotifyProgress);
		}
	}
	// ...
}


// Called every frame
//void UQuestManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
//{
//	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//
//
//
//	// ...
//}

void UQuestManagerComponent::InitializedAvailableQuest()
{
	TArray<FName> AllQuestNames = QuestDataTable->GetRowNames();

	AvailableQuestIDs.Append(AllQuestNames);
}

//퀘스트를 수락하는 함수
void UQuestManagerComponent::AcceptQuest(FName QuestID)
{
	if (!QuestDataTable)
		return;
	//questID와 일치하는 데이터를 찾고, 못찾으면 로그 메세지에 추가정보
	FQuestRow* Data = QuestDataTable->FindRow<FQuestRow>(QuestID, TEXT("QuestManagerComponent/AcceptQuest : Don't find QuestID Data"));

	if (!Data)
		return;

	FActiveQuest NewQuest;
	NewQuest.QuestID = QuestID; 

	//퀘스트에서 필요한 내용들
	//ex) CurrentCounts[0] -> 가지고 있는 열쇠 갯수, CureentCounts[1] -> 가야하는 곳
	NewQuest.CurrentCounts.Init(0, Data->Objectives.Num());

	//이미 완료했거나 진행중이라면 return
	if (!AvailableQuestIDs.Find(QuestID))
	{
		return;
	}

	ActiveQuests.Add(QuestID, NewQuest); // 진행중인 퀘스트에 새로 추가한 퀘스트 넣어주고
	AvailableQuestIDs.Remove(QuestID); //진행가능한 퀘스트에서 제거
	isChangeActiveQuest = true;
}

void UQuestManagerComponent::NotifyProgress(EQuestType QuestType, FName TargetID, int32 Amount)
{
	bool HaveCompleteQuest = false;

	UE_LOG(LogTemp, Warning, TEXT("Quest Signal Received! ID: %s"), *TargetID.ToString());
	for (TPair<FName,FActiveQuest>& Quest : ActiveQuests) ///ex) 어떤 아이템을 먹게되면 플레이어가 현재 진행중인 퀘스트들을 전부 순회
	{
		FName QuestID = Quest.Key;
		// 데이터 테이블 원본 대조
		FQuestRow* Data = QuestDataTable->FindRow<FQuestRow>(QuestID, TEXT(""));
		if (!Data) continue;

		for (int32 i = 0; i < Data->Objectives.Num(); ++i) // 진행중인 퀘스트내부에 필요한 모든 것들을 또 다시 순회
		{
			if (Data->Objectives[i].QuestType == QuestType && Data->Objectives[i].TargetID == TargetID) // 타입도 맞고 해당 타켓이면
			{
				// 정보 갱신
				Quest.Value.CurrentCounts[i] += Amount;
				isChangeActiveQuest = true; // 퀘스트가 변화가 있음을 알림
				//OnQuestUpdated내에 있는 모든 함수 호출
				OnQuestUpdated.Broadcast();

				/*UE_LOG(LogTemp, Warning, TEXT("%s 진행도: %d / %d"),
					*TargetID.ToString(), Quest.CurrentCounts[i], Data->Objectives[i].RequiredCount);*/

				if (Quest.Value.IsComplete(Data))
				{
					CompletedQuestIDs.Add(QuestID);
					HaveCompleteQuest = true;
					UE_LOG(LogTemp, Error, TEXT("퀘스트 완료: %s!"), *Data->QuestName.ToString());
				}
			}
		}
	}

	//반복문에서 찾은 완료된 퀘스트를 진행중 퀘스트에서 제거
	if (HaveCompleteQuest)
	{
		for (auto& WillDeleteQuestID : CompletedQuestIDs)
		{
			ActiveQuests.Remove(WillDeleteQuestID);
		}
	}
}

//TArray<FText> UQuestManagerComponent::GetActiveQuestDescriptions()
//{
//	TArray<FText> Descriptions;
//
//	for (const FActiveQuest& Quest : ActiveQuests)
//	{
//		FQuestRow* Data = QuestDataTable->FindRow<FQuestRow>(Quest.QuestID, TEXT(""));
//		if (!Data) continue;
//
//		// 예: [Quest: 검은 형체 Kill] 검은 형체 (3 / 10)
//		for (int32 i = 0; i < Data->Objectives.Num(); ++i)
//		{
//			FString Description = FString::Printf(TEXT("[%s] %s (%d / %d)"),
//				*Data->QuestName.ToString(),
//				*Data->Objectives[i].TargetID.ToString(),
//				Quest.CurrentCounts[i],
//				Data->Objectives[i].RequiredCount);
//
//			Descriptions.Add(FText::FromString(Description));
//		}
//	}
//	return Descriptions;
//}

void UQuestManagerComponent::SaveQuestData()
{
	// 세이브 객체 생성
	UQuestSaveGame* SaveInstance = Cast<UQuestSaveGame>(UGameplayStatics::CreateSaveGameObject(UQuestSaveGame::StaticClass()));
	
	// 현재 매니저에 있는 퀘스트 진행 데이터를 복사
	SaveInstance->SavedActiveQuests = ActiveQuests;
	SaveInstance->SavedCompletedQuestIDs = CompletedQuestIDs;

	//파일로 저장
	UGameplayStatics::SaveGameToSlot(SaveInstance, TEXT("QuestDataSlot"), 0);

}

void UQuestManagerComponent::LoadQuestData()
{
	// 세이브파일이 있는지 검토
	if (!UGameplayStatics::DoesSaveGameExist(TEXT("QuestDataSlot"), 0))
		return;

	UQuestSaveGame* LoadInstance = Cast<UQuestSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("QuestDataSlot"), 0));
	if (!LoadInstance)
		return;

	ActiveQuests = LoadInstance->SavedActiveQuests;
	CompletedQuestIDs = LoadInstance->SavedCompletedQuestIDs;

}

void UQuestManagerComponent::AutoSave()
{
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle,
		this,
		&UQuestManagerComponent::SaveQuestData,
		60.0f,
		true
	);
}



// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableObject/LockDoor.h"
#include "PlayerCharacter.h"
#include "Components/InventoryComponent.h"
#include "Kismet/GameplayStatics.h" // UGameplayStatics 사용을 위해 필요

ALockDoor::ALockDoor()
{
    // RequiredItemID와 RequiredItemAmount의 기본값 설정 (선택 사항)
    RequiredItemID = FName("Key");
    RequiredItemAmount = 1;
}

void ALockDoor::BeginPlay()
{
    Super::BeginPlay();

    // 상호작용 데이터 초기화
    InteractableData.InteractionDuration = 0.0f; // 즉시 상호작용
    InteractableData.Action = FText::FromString("Unlock");
    InteractableData.Name = FText::FromString("the Door");
}


void ALockDoor::HandleInteraction(APlayerCharacter* PlayerCharacter)
{
    if (!PlayerCharacter) return;

    // 1. 문이 열려있는 상태이거나, 이미 영구적으로 잠금 해제된 상태라면,
    //    조건 검사 없이 바로 부모 클래스의 여닫기 로직을 실행합니다.
    if (bIsOpen || bIsPermanentlyUnlocked) // ⬅️ 영구 해제 상태 검사 추가
    {
        Super::HandleInteraction(PlayerCharacter);
        return;
    }

    // 2. 문이 닫혀있고, 영구 해제되지 않은 상태라면, 잠금 해제 조건을 검사합니다.
    if (CanUnlockDoor(PlayerCharacter))
    {
        UInventoryComponent* Inventory = PlayerCharacter->FindComponentByClass<UInventoryComponent>();
        Inventory->RemoveItemByID(RequiredItemID, RequiredItemAmount); // 아이템 제거


        // 2-1. 잠금 해제 성공: 영구 해제 상태로 변경
        bIsPermanentlyUnlocked = true; // ⬅️ 영구 해제 상태로 설정
        InteractableData.Action = FText::FromString("Close");

        // 2-2. 부모(APushPullDoor)의 상호 작용 로직을 실행 (문을 엽니다).
        Super::HandleInteraction(PlayerCharacter);

        // 2-3. 잠금 해제 성공 메시 (선택 사항)
        FText UnlockMsg = FText::Format(FText::FromString("{0}을(를) 사용하여 문을 잠금 해제했습니다!"), FText::FromName(RequiredItemID));
        // ⚠️ HUD 메시지 표시 로직
        UE_LOG(LogTemp, Warning, TEXT("%s"), *UnlockMsg.ToString());
    }
    else
    {
        // 3. 잠금 해제 실패: 메시지 표시 및 문은 열리지 않음

        FText FailMsg = FText::Format(
            FText::FromString("문을 열려면 {0}이(가) {1}개 필요합니다. (현재 부족)"),
            FText::FromName(RequiredItemID),
            FText::AsNumber(RequiredItemAmount)
        );

        // ⚠️ HUD 메시지 표시 로직
        UE_LOG(LogTemp, Warning, TEXT("%s"), *FailMsg.ToString());
    }
}

bool ALockDoor::CanUnlockDoor(APlayerCharacter* PlayerCharacter) const
{
    if (!PlayerCharacter) return false;

    // 1. 플레이어의 인벤토리 컴포넌트 가져오기
    UInventoryComponent* Inventory = PlayerCharacter->FindComponentByClass<UInventoryComponent>();

    if (!Inventory)
    {
        UE_LOG(LogTemp, Error, TEXT("ALockDoor: PlayerCharacter에 UInventoryComponent가 없습니다."));
        return false;
    }

    // 2. 요구 사항 아이템 ID가 유효한지 확인
    if (RequiredItemID == NAME_None || RequiredItemAmount <= 0)
    {
        // 요구 사항이 없으면 항상 열 수 있다고 가정
        return true;
    }

    // 3. 인벤토리에 필요한 수량이 있는지 확인 (이전 단계에서 구현한 함수 사용)
    const int32 CurrentAmount = Inventory->GetItemAmountByID(RequiredItemID);

    return CurrentAmount >= RequiredItemAmount;
}
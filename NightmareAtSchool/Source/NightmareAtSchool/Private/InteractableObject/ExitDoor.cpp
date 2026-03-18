// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableObject/ExitDoor.h"
#include "PlayerCharacter.h"
#include "Components/InventoryComponent.h"
#include "Kismet/GameplayStatics.h" // UGameplayStatics 사용을 위해 필요

AExitDoor::AExitDoor()
{
}

void AExitDoor::BeginPlay()
{
    Super::BeginPlay();

    // 상호작용 데이터 초기화
    InteractableData.InteractionDuration = 0.0f; // 즉시 상호작용
    InteractableData.Action = FText::FromString("Unlock");
    InteractableData.Name = FText::FromString("the Door");
}

void AExitDoor::HandleInteraction(APlayerCharacter* PlayerCharacter)
{
    if (!PlayerCharacter) return;

    // 1. 문이 열려있는 상태이거나, 이미 영구적으로 잠금 해제된 상태라면,
    //    조건 검사 없이 바로 부모 클래스의 여닫기 로직을 실행합니다.
    if (bIsOpen || bIsPermanentlyUnlocked) // ⬅️ 영구 해제 상태 검사 추가
    {
        Super::HandleInteraction(PlayerCharacter);

        // 등록된 모든 리스너에게 이벤트 전송 (블루프린트의 Call)
        OnDoorOpened.Broadcast();
        return;
    }
}

// 강제로 문을 열어줌
void AExitDoor::UnlockDoor()
{
    bIsPermanentlyUnlocked = true;
}

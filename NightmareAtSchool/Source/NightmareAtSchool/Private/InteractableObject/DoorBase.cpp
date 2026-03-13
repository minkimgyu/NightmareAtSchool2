// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableObject/DoorBase.h"

#include "InteractionInterface.h" // APlayerCharacter.h에서 사용되던 인터페이스

#include "Components/StaticMeshComponent.h"


#include "PlayerCharacter.h" // Interact 함수에서 사용

ADoorBase::ADoorBase()
{
    PrimaryActorTick.bCanEverTick = true;

    // 컴포넌트 설정
    Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    RootComponent = Root;

    DoorFrameMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
    DoorFrameMesh->SetupAttachment(Root);

    // 기본 상태 초기화
    bIsOpen = false;

    UE_LOG(LogTemp, Log, TEXT("Name1: %s Desc1: %s"),
        *InteractableData.Name.ToString(),
        *InteractableData.Action.ToString());
}


// BeginPlay 함수 정의
void ADoorBase::BeginPlay()
{
    Super::BeginPlay(); // 부모 클래스의 BeginPlay 호출은 필수입니다.

    // 상호작용 데이터 초기화
    InteractableData.InteractionDuration = 0.0f; // 즉시 상호작용
    InteractableData.Action = FText::FromString("Open");
    InteractableData.Name = FText::FromString("the Door");

    UE_LOG(LogTemp, Log, TEXT("Name: %s Desc: %s"), 
        *InteractableData.Name.ToString(), 
        *InteractableData.Action.ToString());

    // ADoorBase에서 BeginPlay 시 실행할 초기화 로직 (필요하다면)
}


// Tick 함수 정의
void ADoorBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime); // 부모 클래스의 Tick 호출은 필수입니다.

    // ADoorBase에서 Tick 시 실행할 로직 (예: 문 애니메이션 업데이트 등)
}

void ADoorBase::Interact(APlayerCharacter* PlayerCharacter)
{
    // 문 상호작용을 처리하는 핵심 로직 호출
    HandleInteraction(PlayerCharacter);
}

void ADoorBase::BeginFocus()
{
    // 메시 아웃라인이나 강조 효과를 처리할 수 있습니다.
    //DoorFrameMesh->SetRenderCustomDepth(true);
}

void ADoorBase::EndFocus()
{
    // 강조 효과를 제거합니다.
    //DoorFrameMesh->SetRenderCustomDepth(false);
}

// ADoorBase의 HandleInteraction 기본 구현 (빈 함수 또는 간단한 로직)
void ADoorBase::HandleInteraction(APlayerCharacter* PlayerCharacter)
{
    // HUD 업데이트
   /* if (PlayerCharacter)
    {
        PlayerCharacter->UpdateInteractionWidget();
    }*/

    // ADoorBase에서는 아무것도 하지 않고 자식 클래스가 구현하도록 비워둡니다.
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableObject/SlidingDoor.h"

#include "InteractableObject/DoorBase.h"

#include "InteractionInterface.h"
#include "InteractorInterface.h"

#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h" // USceneComponent 사용을 위해 필요
#include "Components/AudioComponent.h"

#include "Kismet/GameplayStatics.h" // PlaySoundAtLocation 사용을 위해 필요

#include "TimerManager.h"


// Sets default values
ASlidingDoor::ASlidingDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    LeftDoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftDoorMesh"));
    LeftDoorMesh->SetupAttachment(Root);

    RightDoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightDoorMesh"));
    RightDoorMesh->SetupAttachment(Root);

    // OpenPositionMarker 컴포넌트 생성
    OpenPositionMarkerRight = CreateDefaultSubobject<USceneComponent>(TEXT("OpenPositionMarkerRight"));
    OpenPositionMarkerLeft = CreateDefaultSubobject<USceneComponent>(TEXT("OpenPositionMarkerLeft"));
    // Root에 부착
    OpenPositionMarkerLeft->SetupAttachment(RootComponent);
    OpenPositionMarkerRight->SetupAttachment(RootComponent);

    // Marker는 문이 열릴 때 DoorMesh가 도달해야 할 최종 위치를 지정합니다.
    // Blueprint에서 이 Marker의 위치를 조정하여 문이 열리는 거리를 설정합니다.

    //오디오 컴포넌트 생성
    AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("MonsterAudioComp"));

    AudioComp->SetupAttachment(RootComponent);

    AudioComp->bAutoActivate = false;
}

void ASlidingDoor::BeginFocus()
{
    Super::BeginFocus();

    // 메시 아웃라인이나 강조 효과를 처리할 수 있습니다.
    LeftDoorMesh->SetRenderCustomDepth(true);
    RightDoorMesh->SetRenderCustomDepth(true);
}

void ASlidingDoor::EndFocus()
{
    // 강조 효과를 제거합니다.
    Super::EndFocus();

    LeftDoorMesh->SetRenderCustomDepth(false);
    RightDoorMesh->SetRenderCustomDepth(false);
}


// Called when the game starts or when spawned
void ASlidingDoor::BeginPlay()
{
	Super::BeginPlay();
	
    // 문 닫힘 위치 (DoorMesh의 초기 상대 위치)를 저장합니다.
    // 문을 열고 닫을 때 이 위치를 기준으로 보간합니다.
    ClosedLocationLeft = LeftDoorMesh->GetRelativeLocation();
    ClosedLocationRight = RightDoorMesh->GetRelativeLocation();
}

// Called every frame
void ASlidingDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    // 문 상태에 따라 부드러운 슬라이딩 처리
    SlideDoor(DeltaTime);
}

void ASlidingDoor::HandleInteraction(IInteractorInterface* Interactor)
{
    Super::HandleInteraction(Interactor);

    // ADoorBase의 InteractableData 및 bIsOpen 상태만 토글합니다.
    // Tick 함수가 나머지 작업을 처리합니다.

    if (bIsOpen)
    {
        // 닫기 상태로 전환
        bIsOpen = false;
        InteractableData.Action = FText::FromString("Open");

        CloseDoorTimerCancel();
    }
    else
    {
        // 열기 상태로 전환
        bIsOpen = true;
        InteractableData.Action = FText::FromString("Close");

        CloseDoorTimerSetting();
    }

    if (DoorOpenSound)
    {
        // 소리가 캐릭터 위치가 아닌 '문'의 위치에서 나게 하여 입체감을 줍니다.
        //FVector SoundLocation = GetActorLocation();
        //UGameplayStatics::PlaySoundAtLocation(this, DoorOpenSound, SoundLocation);
        AudioComp->Play();
    }

    Interactor->UpdateInteractionWidget(&InteractableData);
}

void ASlidingDoor::SlideDoor(float DeltaTime)
{
    // 1. 목표 위치 설정
    FVector TargetLocationLeft;
    FVector TargetLocationRight;

    if (bIsOpen)
    {
        TargetLocationLeft = OpenPositionMarkerLeft->GetRelativeLocation();
        TargetLocationRight = OpenPositionMarkerRight->GetRelativeLocation();
    }
    else
    {
        //자동닫기 해제
        bisAutoClose = false;

        // 닫힌 상태: BeginPlay에서 저장된 초기 위치를 목표로 합니다.
        TargetLocationLeft = ClosedLocationLeft;
        TargetLocationRight = ClosedLocationRight;
    }


    // 2. 현재 위치 가져오기
    FVector CurrentLocationLeft = LeftDoorMesh->GetRelativeLocation();

    // 3. FMath::VInterpTo를 사용하여 목표 위치로 부드럽게 보간합니다.
    //    DeltaTime * SlideSpeed를 사용하여 프레임 속도에 독립적인 부드러운 이동을 구현합니다.
    FVector NewLocationLeft = FMath::VInterpTo(CurrentLocationLeft, TargetLocationLeft, DeltaTime, SlideSpeed);

    // 4. 문 메시의 위치를 업데이트합니다.
    LeftDoorMesh->SetRelativeLocation(NewLocationLeft);



    // 2. 현재 위치 가져오기
    FVector CurrentLocationRight = RightDoorMesh->GetRelativeLocation();

    // 3. FMath::VInterpTo를 사용하여 목표 위치로 부드럽게 보간합니다.
    //    DeltaTime * SlideSpeed를 사용하여 프레임 속도에 독립적인 부드러운 이동을 구현합니다.
    FVector NewLocationRight = FMath::VInterpTo(CurrentLocationRight, TargetLocationRight, DeltaTime, SlideSpeed);

    // 4. 문 메시의 위치를 업데이트합니다.
    RightDoorMesh->SetRelativeLocation(NewLocationRight);
}

void ASlidingDoor::CloseDoorTimer()
{
    bIsOpen = false;
    AudioComp->Play();
}

void ASlidingDoor::CloseDoorTimerSetting()
{
    float DelayTime = 6.0f; // 2초
    bool bIsLooping = false; // 반복 안 함

    // 타이머 세팅
    GetWorldTimerManager().SetTimer(
        CloseDoorTimerHandle,
        this,
        &ASlidingDoor::CloseDoorTimer, // 실행할 함수 주소
        DelayTime,
        bIsLooping
    );
}

void ASlidingDoor::CloseDoorTimerCancel()
{
    if (GetWorldTimerManager().IsTimerActive(CloseDoorTimerHandle))
    {
        GetWorldTimerManager().ClearTimer(CloseDoorTimerHandle);
    }
}